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


#include <errno.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "actube.h"
#include "wtplist.h"
#include "dtls.h"
#include "cw_log.h"
#include "conf.h"
#include "sock.h"

#include "socklist.h"

#include "db.h"

int ac_run();

void alive_thread(void *data)
{
	while(1){
		sleep(5);
		db_ping();
	}
}


int main (int argc, const char * argv[]) 
{
//	send_image_file(0,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAP.tar");


	cw_log_name="AC-Tube";

	read_config("ac.conf");
	cw_log_debug_level=conf_debug_level;


	
	cw_log(LOG_INFO,"Starting AC-Tube, Name=%s, ID=%s",conf_acname,conf_acid);

//	cw_dbg_opt_level=   DBG_CW_MSGELEM_DMP | 
//			DBG_CW_MSGELEM | DBG_CW_PKT| DBG_CW_RFC | DBG_ERR | DBG_CW_MSG | DBG_DTLS ; //| DBG_ALL;


	cw_dbg_opt_detail=DBG_DETAIL_ASC_DMP;

//	cw_log_dbg(DBG_CW_MSG,"Hello %s","World");



	db_init();
	db_start();
	db_ping();

	pthread_t alth;
	pthread_create (&alth, NULL, alive_thread, (void *)0);
	

#ifdef WITH_DTLS
	dtls_init();
#endif	
	if (!socklist_init())
		goto errX;

	if (!wtplist_init())
		goto errX;


	int rc = ac_run();
errX:
	wtplist_destroy();
	socklist_destroy();
	return rc;
}

void process_ctrl_packet(int  index, struct sockaddr * addr, uint8_t * buffer, int len);
#define AC_PROTO_CAPWAP 0
#define AC_PROTO_LWAPP 1




int ac_run()
{

	if (!conf_listen_addrs_len){
		cw_log(LOG_ERR,"Fatal error: No listen addresses found.");
		return 1;
	}



	/* it is important to create the unicast sockets first, 
	 * because when we create the mcast an bcast sockets next 
	 * we will look for already created sockets to find a
	 * good unicast reply socket */

	int i;
	for(i=0; i<conf_listen_addrs_len; i++){
		socklist_add_unicast(conf_listen_addrs[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp) 
			socklist_add_unicast(conf_listen_addrs[i],conf_lw_control_port,AC_PROTO_LWAPP);
#endif 
	}

	if (socklist_len==0){
		cw_log(LOG_ERR,"Fatal error: Could not setup any listen socket");
		return 1;
	}

	/* create multicast sockets */
	for (i=0; i<conf_mcast_groups_len;i++){
		socklist_add_multicast(conf_mcast_groups[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp) 
			socklist_add_multicast(conf_mcast_groups[i],conf_lw_control_port,AC_PROTO_LWAPP);
#endif 

	}

	/* broadcast sockety ipv4 only */
	for (i=0; i<conf_bcast_addrs_len;i++){
		socklist_add_broadcast(conf_bcast_addrs[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
//		printf("Adding %d\n",socklist_len);
		if (conf_lwapp) 
			socklist_add_broadcast(conf_bcast_addrs[i],conf_lw_control_port,AC_PROTO_LWAPP);
//		printf ("SI %d, PROTO: %d\n",socklist_len-1,socklist[socklist_len-1].ac_proto);
#endif 
	}



	get_acinfo();


	while(1){

		/* prepare fdset */
		fd_set fset;
		int max = 0;
		FD_ZERO(&fset);
		for (i=0; i<socklist_len; i++){
			FD_SET(socklist[i].sockfd,&fset);
			if (socklist[i].sockfd>max)
				max=socklist[i].sockfd;
		}

		/* wait for an event */
		int n;
	        while((n=select(max+1, &fset, NULL, NULL, NULL)) < 0) {
			if (errno != EINTR) 
				return n;

	        }

		/* process the received packet */
		for( i=0; i<socklist_len; i++){

			if (!FD_ISSET(socklist[i].sockfd,&fset))
				continue;

			struct sockaddr_storage srcaddr;
			socklen_t sockaddrlen;

			memset(&srcaddr,0,sizeof(struct sockaddr_storage));
			sockaddrlen = sizeof(struct sockaddr_storage);

			uint8_t buffer[4096];
			int len = sock_receive(socklist[i].sockfd,
					buffer, sizeof(buffer),
					0,
					(struct sockaddr*)&srcaddr, &sockaddrlen);

			process_ctrl_packet(i, (struct sockaddr*)&srcaddr,buffer,len);
		}

	}


	/* close and free all sockts */
	for(i=0; i<socklist_len; i++){
//		close(socklist[i]);
	}
	free(socklist);
	return 0;
}


void process_cw_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{

	int sock = socklist[index].reply_sockfd;

	char hdrstr[1024];
	hdr_print(hdrstr,buffer,len);
	cw_dbg(DBG_CW_PKT_IN,"Header for packet from %s\n%s",sock_addr2str(addr),hdrstr);
	


	/* first of all check preamble */
	int preamble = CWTH_GET_PREAMBLE(buffer);

#ifdef WITH_DTLS
	if (preamble != CAPWAP_PACKET_PREAMBLE && preamble != CAPWAP_DTLS_PACKET_PREAMBLE){
#else
	if (preamble != CAPWAP_PACKET_PREAMBLE ){
#endif
		cw_dbg(DBG_CW_PKT_ERR,"Discarding packet, wrong preamble, preamble = 0x%01X",preamble);
		return;
	}

	wtplist_lock();
	struct wtpman * wtpman = wtplist_get(addr);
	if (!wtpman){

		wtpman = wtpman_create(index,addr);

		if (!wtpman ){
			cw_log(LOG_ERR,"Error creating wtpman: %s",strerror(errno));
			wtplist_unlock();
			return;
		}
		

		if (!wtplist_add(wtpman)){
			cw_log(LOG_ERR,"Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		wtpman_start(wtpman,preamble & 0xf);
	}


	wtpman_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}



void process_lw_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{
	int sock = socklist[index].reply_sockfd;

	uint8_t * m = buffer+6;
	uint32_t val = ntohl(*((uint32_t*)(m)));


	printf ("VAL: %08X\n",val);

	

	/* first of all check preamble */
	int version = LWTH_GET_VERSION(m);

	if (version != LW_VERSION){
		cw_log_debug1("Discarding LWAPP packet, wrong verson");
		return;
	}

	int l = LWTH_GET_LENGTH(m);
	printf ("LEN = %d\n",l);

	if (l+12 != len){
		cw_log_debug1("Discarding LWAPP packet, wrong length");
		return;
	}

	wtplist_lock();
	struct wtpman * wtpman = wtplist_get(addr);
	if (!wtpman){

		wtpman = wtpman_create(index,addr);

		if (!wtpman ){
			cw_log(LOG_ERR,"Error creating wtpman: %s",strerror(errno));
			wtplist_unlock();
			return;
		}
		

		if (!wtplist_add(wtpman)){
			cw_log(LOG_ERR,"Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		wtpman_lw_start(wtpman);
	}

	wtpman_lw_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}

















void process_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{

#ifdef WITH_CW_LOG_DEBUG
	char str[100];
	sock_addrtostr(addr,str,100);
	cw_dbg(DBG_CW_PKT_IN,"Received packet from %s, len = %i, via %s\n",str,len,
			socklist[index].type==SOCKLIST_UNICAST_SOCKET ? "unicast":"bcast/mcast");
	cw_log_debug2_dump(buffer,len,"Packet data for packet, recevied from %s",str);
#endif	

	switch (socklist[index].ac_proto){
		case AC_PROTO_CAPWAP:
			process_cw_ctrl_packet(index,addr,buffer,len);
			return;
		case AC_PROTO_LWAPP:
			process_lw_ctrl_packet(index,addr,buffer,len);
			return;
	}

}



