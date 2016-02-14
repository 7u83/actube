/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
//#include <netinet/in.h>

#include "actube.h"
#include "wtplist.h"
#include "capwap/dtls.h"
#include "capwap/log.h"
#include "capwap/dbg.h"
#include "capwap/sock.h"
#include "conf.h"

#include "socklist.h"

#include "db.h"
#include "capwap/capwap_items.h"
#include "capwap/capwap_cisco.h"
#include "capwap/capwap_80211.h"
#include "capwap/action.h"
#include "capwap/capwap_crypto.h"
#include "capwap/mbag.h"


#include "ac.h"
#include "capwap/format.h"

int ac_run();


static void *alive_thread(void *data)
{
	/* Ping the database every 5 seconds */
	while (1) {
		sleep(5);
		db_ping();
	}
}

#include <getopt.h>
static int parse_args(int argc, char *argv[])
{
	int getopt_ret, option_index;

	static struct option long_options[] = {
		{"version", optional_argument, NULL, 'v'},
		{0, 0, 0, 0}
	};
	int o;
	while ((o = getopt_long(argc, argv, "v", long_options, &option_index)) != -1) {
		switch (o) {
			case 0:
				break;
			case 'v':
				printf("AC-Tube 0.01, %s\n", SYS_ARCH);
				exit(0);
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int rc = 0;

	/* parse arguments */
	parse_args(argc, argv);

	cw_log_name = "AC-Tube";

	read_config("ac.conf");

	/* Show debug options if there are any set */
	if (cw_dbg_opt_level)
		cw_log(LOG_INFO, "Debug Options: %08X", cw_dbg_opt_level);

	/* XXX Hard coded debug settigns, set it by config in the future */
	cw_dbg_opt_display = DBG_DISP_ASC_DMP | DBG_DISP_COLORS;

	/* Warn, if the "secret" debugging feature for 
	   developers is turned on ;) */
	DBGX("Attention! %s", "DBGX is ON!");


	/* Initialize the database */
	if (!db_init())
		goto errX;

	/* Start the database */
	if (!db_start())
		goto errX;

	db_ping();


	/* Start a database "pinger thread", which inserts
	   every xx seconds a timestamp into the DB */
	pthread_t alth;
	pthread_create(&alth, NULL, alive_thread, NULL);

	/* Init DTLS library */
	dtls_init();

	int regn;

	/* Load CAPWAP base protocol */
	if (conf_capwap_mode == CW_MODE_CIPWAP) {
		cw_dbg(DBG_INFO, "Loading CIPWAP Actions ...");
		regn = cw_register_actions_cipwap_ac(&capwap_actions);
	} else {
		cw_dbg(DBG_INFO, "Loading standard CAPWAP Actions ...");
		regn = cw_register_actions_capwap_ac(&capwap_actions);
	}

	/* Load bindings */
	cw_dbg(DBG_INFO, "Loading 802.11 Bindings ...");
//      regn += cw_register_actions_capwap_80211_ac(&capwap_actions);

	cw_dbg(DBG_INFO, "Registered %d protocol actions and strings.", regn);



	ac_global_init();
	if (!socklist_init())
		goto errX;

	if (!wtplist_init())
		goto errX;




	cw_log(LOG_INFO, "Starting AC-Tube, Name=%s, ID=%s", conf_acname, conf_acid);
	rc = ac_run();
      errX:
	/* XXX There is more cleanup to do */
	wtplist_destroy();
	socklist_destroy();
	return rc;
}







void process_ctrl_packet(int index, struct sockaddr *addr, uint8_t * buffer, int len);
#define AC_PROTO_CAPWAP 0
#define AC_PROTO_LWAPP 1




int ac_run()
{

	if (!conf_listen_addrs_len) {
		cw_log(LOG_ERR, "Fatal error: No listen addresses found.");
		return 1;
	}



	/* it is important to create the unicast sockets first, 
	 * because when we create the mcast an bcast sockets next 
	 * we will look for already created sockets to find a
	 * good unicast reply socket */

	int i;
	for (i = 0; i < conf_listen_addrs_len; i++) {
		socklist_add_unicast(conf_listen_addrs[i], conf_control_port,
				     AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp)
			socklist_add_unicast(conf_listen_addrs[i], conf_lw_control_port,
					     AC_PROTO_LWAPP);
#endif
	}

	if (socklist_len == 0) {
		cw_log(LOG_ERR, "Fatal error: Could not setup any listen socket");
		return 1;
	}

	/* create multicast sockets */
	for (i = 0; i < conf_mcast_groups_len; i++) {
		socklist_add_multicast(conf_mcast_groups[i], conf_control_port,
				       AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp)
			socklist_add_multicast(conf_mcast_groups[i], conf_lw_control_port,
					       AC_PROTO_LWAPP);
#endif

	}

	/* broadcast sockety ipv4 only */
	for (i = 0; i < conf_bcast_addrs_len; i++) {
		socklist_add_broadcast(conf_bcast_addrs[i], conf_control_port,
				       AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
//              printf("Adding %d\n",socklist_len);
		if (conf_lwapp)
			socklist_add_broadcast(conf_bcast_addrs[i], conf_lw_control_port,
					       AC_PROTO_LWAPP);
//              printf ("SI %d, PROTO: %d\n",socklist_len-1,socklist[socklist_len-1].ac_proto);
#endif
	}



	//get_acinfo();


	while (1) {

		/* prepare fdset */
		fd_set fset;
		int max = 0;
		FD_ZERO(&fset);
		for (i = 0; i < socklist_len; i++) {
			FD_SET(socklist[i].sockfd, &fset);
			if (socklist[i].sockfd > max)
				max = socklist[i].sockfd;
		}

		/* wait for an event */
		int n;
		while ((n = select(max + 1, &fset, NULL, NULL, NULL)) < 0) {
			if (errno != EINTR)
				return n;

		}

		/* process the received packet */
		for (i = 0; i < socklist_len; i++) {

			if (!FD_ISSET(socklist[i].sockfd, &fset))
				continue;

			struct sockaddr_storage srcaddr;
			socklen_t sockaddrlen;

			memset(&srcaddr, 0, sizeof(struct sockaddr_storage));
			sockaddrlen = sizeof(struct sockaddr_storage);

			uint8_t buffer[4096];
			int len = sock_receive(socklist[i].sockfd,
					       buffer, sizeof(buffer),
					       0,
					       (struct sockaddr *) &srcaddr,
					       &sockaddrlen);

			process_ctrl_packet(i, (struct sockaddr *) &srcaddr, buffer, len);
		}

	}


	/* close and free all sockts */
	for (i = 0; i < socklist_len; i++) {
//              close(socklist[i]);
	}
	free(socklist);
	return 0;
}


void process_cw_ctrl_packet(int index, struct sockaddr *addr, uint8_t * buffer, int len)
{

	/* first of all check preamble */
	int preamble = cw_get_hdr_preamble(buffer);

	if (preamble != CAPWAP_PACKET_PREAMBLE && preamble != CAPWAP_DTLS_PACKET_PREAMBLE) {
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, wrong preamble, preamble = 0x%01X",
		       sock_addr2str(addr), preamble);
		return;
	}


	wtplist_lock();
	struct wtpman *wtpman = wtplist_get(addr);
	if (!wtpman) {

		wtpman = wtpman_create(index, addr);


		if (!wtpman) {
			cw_log(LOG_ERR, "Error creating wtpman: %s", strerror(errno));
			wtplist_unlock();
			return;
		}


		if (!wtplist_add(wtpman)) {
			cw_log(LOG_ERR, "Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		wtpman_start(wtpman, preamble & 0xf);
	}


	wtpman_addpacket(wtpman, buffer, len);
	wtplist_unlock();
}



void process_lw_ctrl_packet(int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	//int sock = socklist[index].reply_sockfd;

	uint8_t *m = buffer + 6;
	uint32_t val = ntohl(*((uint32_t *) (m)));


	printf("VAL: %08X\n", val);



	/* first of all check preamble */
	int version = LWTH_GET_VERSION(m);

	if (version != LW_VERSION) {
//              cw_log_debug1("Discarding LWAPP packet, wrong verson");
		return;
	}

	int l = LWTH_GET_LENGTH(m);
	printf("LEN = %d\n", l);

	if (l + 12 != len) {
		//      cw_log_debug1("Discarding LWAPP packet, wrong length");
		return;
	}

	wtplist_lock();
	struct wtpman *wtpman = wtplist_get(addr);
	if (!wtpman) {

		wtpman = wtpman_create(index, addr);

		if (!wtpman) {
			cw_log(LOG_ERR, "Error creating wtpman: %s", strerror(errno));
			wtplist_unlock();
			return;
		}


		if (!wtplist_add(wtpman)) {
			cw_log(LOG_ERR, "Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		//wtpman_lw_start(wtpman);
	}
	//wtpman_lw_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}





void process_ctrl_packet(int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	switch (socklist[index].ac_proto) {
		case AC_PROTO_CAPWAP:
			process_cw_ctrl_packet(index, addr, buffer, len);
			return;
		case AC_PROTO_LWAPP:
			process_lw_ctrl_packet(index, addr, buffer, len);
			return;
	}
}
