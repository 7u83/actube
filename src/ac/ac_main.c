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

#include <stdlib.h>
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
#include "capwap_items.h"
#include "capwap_cisco.h"



int ac_run();

void alive_thread(void *data)
{
	while(1){
		sleep(5);
		db_ping();
	}
}

#include "cw_action.h"

cw_actionlist_t the_tree;

int dstart(struct conn *conn,struct cw_action_t * a,uint8_t *data,int len)
{
	printf("DISCO STart Action!!\n");

	uint8_t * rmac = cw_get_hdr_rmac(data);
	printf ("This msg is for %s\n",sock_hwaddr2str(bstr_data(rmac), bstr_len(rmac)));
}

int readelem_discovery_type(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
	printf("Discovery Type = %d\n",*data);
	cw_itemstore_set_byte(conn->itemstore,CW_ITEM_DISCOVERY_TYPE,*data);
}

/*
int readelem_vendor_specific_payload(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
        cw_action_t as,*af;
	as = *a;

	as.vendor_id = cw_get_dword(data);
	as.elem_id = cw_get_word(data+4);
	printf("Vendor Specific: %d, %d\n",as.vendor_id,as.elem_id);

 	af = cw_actionlist_get(conn->msgtr,&as);



	if (!af) {
		printf("Msg unknown\n");
		return 0;
	}

	printf("Found\n");

	if (af->start) {
		af->start(conn,af,data+6,len-6);
	}


	return 1;
}
*/


int readelem_cisco_rad_name(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
	printf("Here is the rad name reader\n");
	int i;

	for (i=0; i<len; i++) {
		printf("%c",data[i]);
	}
	printf("\n");
}





int main (int argc, const char * argv[]) 
{


cw_actionlist_t t = cw_actionlist_create();
the_tree=t;


cw_action_t discovery_actions[] = {
{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST,-1,
  dstart,0 },

{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_DISCOVERY_TYPE,
  readelem_discovery_type,0 },

{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
  cw_in_vendor_specific_payload,0 },

{ CW_VENDOR_ID_CISCO,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_CISCO_RAD_NAME,
  cw_in_wtp_name,0 },


{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_WTP_BOARD_DATA,
  cw_in_wtp_board_data,0 },

/*
{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_WTP_DESCRIPTOR},
{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_WTP_FRAME_TUNNEL_MODE},
{ 0,0,CW_STATE_DISCOVERY,CW_MSG_DISCOVERY_REQUEST, CW_ELEM_WTP_MAC_TYPE},

*/

{ 0,0,0}
};



cw_action_t join_actions[] = {

{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST,-1,0,0},
{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST, CW_ELEM_DISCOVERY_TYPE},
{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST, CW_ELEM_WTP_BOARD_DATA},
{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST, CW_ELEM_WTP_DESCRIPTOR},
{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST, CW_ELEM_WTP_FRAME_TUNNEL_MODE},
{0,0,CW_STATE_JOIN,CW_MSG_JOIN_REQUEST, CW_ELEM_WTP_MAC_TYPE},
{0}


};


cw_register_actions(t,discovery_actions);
cw_register_actions(t,join_actions);


#define CW_NAME_DISCOVER_REQUEST "Discovery Request"

/*
int i;
for(i=0; discovery[i].capwap_state!=CW_STATE_MAX; i++){
	printf("State: %d MSG_ID: %d ELEM_ID: %d\n",discovery[i].capwap_state,discovery[i].msg_id,discovery[i].elem_id);
	void * rc = msgtree_add(t,&(discovery[i]));

	printf("Added to tree %p\n",rc);

}
*/


/*
e.capwap_state=0;
e.msg_id = CW_MSG_DISCOVERY_REQUEST;
e.elem_id = CW_ELEM_AC_NAME;
e.name = "AC Name";

msgtree_add(t,&e);
*/




//	send_image_file(0,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAP.tar");


	cw_log_name="AC-Tube";

	read_config("ac.conf");
//	cw_log_debug_level=conf_debug_level;

	
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
	int preamble = cw_get_hdr_preamble(buffer);

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
//		cw_log_debug1("Discarding LWAPP packet, wrong verson");
		return;
	}

	int l = LWTH_GET_LENGTH(m);
	printf ("LEN = %d\n",l);

	if (l+12 != len){
	//	cw_log_debug1("Discarding LWAPP packet, wrong length");
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
	cw_dbg(DBG_CW_PKT_IN,"Received packet from %s, len = %i, via %s\n",sock_addr2str(addr),len,
			socklist[index].type==SOCKLIST_UNICAST_SOCKET ? "unicast":"bcast/mcast");

	cw_dbg_dmp(DBG_CW_PKT_DMP,buffer,len,"Dump ...");
//	cw_dbg_dmp(buffer,len,"Packet data for packet, recevied from %s",str);
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



