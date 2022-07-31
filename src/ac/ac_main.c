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


#include "actube.h"
#include "wtplist.h"
#include "cw/dtls.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/sock.h"
#include "conf.h"

#include "socklist.h"

#include "db.h"


#include "cw/capwap_crypto.h"



#include "ac.h"
#include "cw/format.h"


#include "dataman.h"

#include "cw/cfg.h"

int ac_run();


#include <getopt.h>



struct bootcfg {
	const char * cfgfilename;
};

static int parse_args (int argc, char *argv[], struct bootcfg * bootcfg)
{
	int c;
	opterr = 1;
	
	bootcfg->cfgfilename = "config.atv";
	
	while ( (c = getopt (argc, argv, "vc:d:p:")) != -1) {
		
		switch (c) {
			case 'c':
				bootcfg->cfgfilename = optarg;
				break;
			case 'v':
				printf("AC-Tube version 0.0.1, %s\n", SYS_ARCH);
				exit(EXIT_SUCCESS);
				break;
			case 'd':{
				int b = cw_strlist_get_id(cw_dbg_strings, optarg);
				if (b==-1){
					fprintf(stderr,"Invalid debug option: %s\n",optarg);
					exit(EXIT_FAILURE);
				}
				cw_dbg_set_level(b, 1);
				break;
			}

			case 'p':
				cw_mod_set_path(optarg);
				break;
			case '?':
				exit(EXIT_FAILURE);
			default:
			case 'h':
				printf("%s: -vcmh\n",argv[0]);
				exit(EXIT_SUCCESS);
				break;
		}
	}
	return 0;
}



struct cw_DiscoveryCache * discovery_cache;
mavl_t actube_global_cfg;

int ac_global_init()
{
	/* initialize discovery cache */
	discovery_cache = discovery_cache_create(100);
	if (discovery_cache==NULL)
		goto errX;


	return 1;
errX:
	return 0;
}











#include "cw/mod.h"
extern struct cw_Mod * cw_get_mod_ac (const char *name);

#include "cw/mlist.h"


extern void test_sets();
#include "cw/file.h"
#include "cw/val.h"


#include "discovery_cache.h"
/*
	{
		cw_Cfg_t * cfg;
		cfg=cw_cfg_create();
		char *val;

		cw_cfg_load("tube.akv",cfg);

//		cw_cfg_set(cfg,"cisco/tube","99");
//		cw_cfg_set(cfg,"Hello","30");
	
		cw_cfg_dump(cfg);

		val = cw_cfg_get(cfg,"birth");
		if (val){
			printf("Birts: %s\n",val);
		}

		mavl_destroy(cfg);
	}
	cw_Val_t x;
	CW_TYPE_STR->from_str(&x,"hallo");
	x.type->del(&x);	

return 0;	
*/



int main (int argc, char *argv[])
{
	int rc = 0;
	struct bootcfg bootcfg;
	FILE * file;
	mavl_t types_tree, acglobal_cfg;
	const cw_Type_t **ti;


	cw_Cfg_t * global_cfg = NULL;

	/* parse arguments */
	parse_args (argc, argv, &bootcfg);

	global_cfg=cw_cfg_create();
	if (!global_cfg){
		fprintf(stderr,"Can't create global_cfg: %s\n",strerror(errno));
		goto errX;
	}
	
	rc = cw_cfg_load(bootcfg.cfgfilename,global_cfg);
	if (rc)
	{
		if (rc<0)
			fprintf(stderr,"Can't load cfg '%s': %s\n",bootcfg.cfgfilename,strerror(errno));
		goto errX;
	};

	actube_global_cfg = acglobal_cfg;


	cw_log_name = "AC-Tube";
	
		
	start_shell();

	/* Init DTLS library */
	dtls_init();
		
	ac_global_init();
	
	if (!socklist_init())
		goto errX;
	
	if (!wtplist_init())
		goto errX;
		
	if (!dataman_list_init())
		goto errX;
		
	cw_log (LOG_INFO, "Starting AC-Tube, Name=%s, ID=%s", conf_acname, conf_acid);
	rc = ac_run();

errX:
	if (global_cfg)
		mavl_destroy(global_cfg);

	if (discovery_cache)
		discovery_cache_destroy(discovery_cache);

	wtplist_destroy();
	socklist_destroy();
	return rc;
}




void process_ctrl_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len);

void process_cw_data_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len);


int ac_run()
{

	if (!conf_listen_addrs_len) {
		cw_log (LOG_ERR, "Fatal error: No listen addresses found.");
		return 1;
	}
	
	
	
	/* it is important to create the unicast sockets first,
	 * because when we create the mcast an bcast sockets next
	 * we will look for already created sockets to find a
	 * good unicast reply socket */
	
	int i;
	
	for (i = 0; i < conf_listen_addrs_len; i++) {
		char addr[100];
		char port[50];
		int proto;
		
		conf_parse_listen_addr (conf_listen_addrs[i], addr, port, &proto);
		
		socklist_add_unicast (addr, port, proto);
	}
	
	if (socklist_len == 0) {
		cw_log (LOG_ERR, "Fatal error: Could not setup any listen socket");
		return 1;
	}
	
	/* create multicast sockets */
	for (i = 0; i < conf_mcast_groups_len; i++) {
	
		socklist_add_multicast (conf_mcast_groups[i], conf_control_port,
		                        AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		                        
		if (conf_lwapp)
			socklist_add_multicast (conf_mcast_groups[i], conf_lw_control_port,
			                        AC_PROTO_LWAPP);
			                        
#endif
			                        
	}
	
	/* broadcast socket ipv4 only */
	for (i = 0; i < conf_bcast_addrs_len; i++) {
	
		char addr[50], port[50];
		int proto;
		conf_parse_listen_addr (conf_bcast_addrs[i], addr, port, &proto);
		
		socklist_add_broadcast (addr, port, proto);
#ifdef WITH_LWAPP
		
//              printf("Adding %d\n",socklist_len);
		if (conf_lwapp)
			socklist_add_broadcast (conf_bcast_addrs[i], conf_lw_control_port,
			                        AC_PROTO_LWAPP);
			                        
//              printf ("SI %d, PROTO: %d\n",socklist_len-1,socklist[socklist_len-1].ac_proto);
#endif
	}
	
	
	
	/*//get_acinfo();*/
	
	
	while (1) {
	
		/* prepare fdset */
		fd_set fset;
		int max = 0;
		FD_ZERO (&fset);
		
		for (i = 0; i < socklist_len; i++) {
			FD_SET (socklist[i].sockfd, &fset);
			
			if (socklist[i].sockfd > max)
				max = socklist[i].sockfd;
				
			if (socklist[i].data_sockfd) {
				FD_SET (socklist[i].data_sockfd, &fset);
				
				if (socklist[i].sockfd > max)
					max = socklist[i].sockfd;
			}
		}
		
		/* wait for an event */
		int n;
		
		while ( (n = select (max + 1, &fset, NULL, NULL, NULL)) < 0) {
			if (errno != EINTR)
				return n;
				
		}
		
		/* process the received packet */
		for (i = 0; i < socklist_len; i++) {
			uint8_t buffer[4096];
			struct sockaddr_storage srcaddr;
			socklen_t srcaddrlen;
			
			
			if (FD_ISSET (socklist[i].data_sockfd, &fset)) {
			
				int len = sock_receive (socklist[i].data_sockfd,
				                        buffer, sizeof (buffer),
				                        0,
				                        (struct sockaddr *) &srcaddr,
				                        &srcaddrlen);
				                        
				process_cw_data_packet (i, (struct sockaddr *) &srcaddr, buffer, len);
				
			}
			
			
			if (FD_ISSET (socklist[i].sockfd, &fset)) {
			
				int len = sock_receive (socklist[i].sockfd,
				                        buffer, sizeof (buffer),
				                        0,
				                        (struct sockaddr *) &srcaddr,
				                        &srcaddrlen);
				                        
				process_ctrl_packet (i, (struct sockaddr *) &srcaddr, buffer, len);
			}
			
		}
		
	}
	
	return 0;
}


void process_cw_data_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	cw_dbg (DBG_X, "There is a data packet now");
	
	dataman_list_lock();
	cw_dbg (DBG_X, "Dataman list locked, now getting");
	struct dataman * dm = dataman_list_get (socklist[index].data_sockfd, addr);
	cw_dbg (DBG_X, "Dataman list locked, now gotted");
	
	cw_dbg (DBG_INFO, "Packet for dataman %s,%d", sock_addr2str_p (addr, sock_buf), socklist[index].data_sockfd);
	
	if (!dm) {
		cw_dbg (DBG_INFO, "No dataman %s,%d", sock_addr2str_p (addr, sock_buf), socklist[index].data_sockfd);
		dm = dataman_create (socklist[index].data_sockfd, addr);
		
		if (!dm) {
			cw_log (LOG_ERR, "Can't create dataman for packet from %s", sock_addr2str_p (addr, sock_buf));
			return;
		}
		
		dataman_list_add (dm);
		
		dataman_start (dm);
		
		
	}
	
	dataman_list_unlock();
	
	dataman_add_packet (dm, buffer, len);
	
	return;
	
	exit (0);
	
	
	
	
	printf ("Data packet received len = %d\n", len);
	exit (0);
	struct wtpman *wtpman = wtplist_get (addr);
	
	if (!wtpman) {
		cw_dbg (DBG_PKT_ERR, "Discarding packet on data channel from %s - No wtpman found.", sock_addr2str (addr, sock_buf));
		return;
	}
	
	
	wtpman_addpacket (wtpman, buffer, len);
}


void process_cw_ctrl_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	
	/* first of all check preamble */
	int preamble = cw_get_hdr_preamble (buffer);
	
	if (preamble != CAPWAP_PACKET_PREAMBLE && preamble != CAPWAP_DTLS_PACKET_PREAMBLE) {
		cw_dbg (DBG_PKT_ERR,
		        "Discarding packet from %s, wrong preamble, preamble = 0x%01X",
		        sock_addr2str (addr, sock_buf), preamble);
		return;
	}
	
	
	wtplist_lock();
	struct wtpman *wtpman = wtplist_get (addr);


	if (!wtpman) {
	
		wtpman = wtpman_create (index, addr, preamble & 0xf);
		
		
		if (!wtpman) {
			cw_log (LOG_ERR, "Error creating wtpman: %s", strerror (errno));
			wtplist_unlock();
			return;
		}
		
		
		if (!wtplist_add (wtpman)) {
			cw_log (LOG_ERR, "Error adding wtpman: Too many wtp connections");
			wtpman_destroy (wtpman);
			wtplist_unlock();
			return;
		};
		
		wtpman_start (wtpman, preamble & 0xf);
	}
	
	
	wtpman_addpacket (wtpman, buffer, len);
	wtplist_unlock();
}


/*
void process_lw_ctrl_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	//int sock = socklist[index].reply_sockfd;
	
	uint8_t *m = buffer + 6;
	uint32_t val = ntohl (* ( (uint32_t *) (m)));
	
	
	printf ("VAL: %08X\n", val);
	
	
	
	// first of all check preamble 
	int version = LWTH_GET_VERSION (m);
	
	if (version != LW_VERSION) {
//              cw_log_debug1("Discarding LWAPP packet, wrong verson");
		return;
	}
	
	int l = LWTH_GET_LENGTH (m);
	printf ("LEN = %d\n", l);
	
	if (l + 12 != len) {
		//      cw_log_debug1("Discarding LWAPP packet, wrong length");
		return;
	}
	
	wtplist_lock();
	struct wtpman *wtpman = wtplist_get (addr);


	if (!wtpman) {
	
		wtpman = wtpman_create (index, addr);

		if (!wtpman) {
			cw_log (LOG_ERR, "Error creating wtpman: %s", strerror (errno));
			wtplist_unlock();
			return;
		}
		
		
		if (!wtplist_add (wtpman)) {
			cw_log (LOG_ERR, "Error adding wtpman: Too many wtp connections");
			wtpman_destroy (wtpman);
			wtplist_unlock();
			return;
		};
		
		//wtpman_lw_start(wtpman);
	}
	
	//wtpman_lw_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}

*/



void process_ctrl_packet (int index, struct sockaddr *addr, uint8_t * buffer, int len)
{
	switch (socklist[index].ac_proto) {
		case AC_PROTO_CAPWAP:
			process_cw_ctrl_packet (index, addr, buffer, len);
			return;
			
		/*case AC_PROTO_LWAPP:
			process_lw_ctrl_packet (index, addr, buffer, len);
			return;*/
	}
}
