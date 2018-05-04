
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "wtp_interface.h"
#include "wtp_conf.h"

#include "cw/dtls.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/sock.h"
#include "cw/dtls.h"
#include "cw/mavl.h"

#include "wtp.h"


/*
#define acinfo_log acinfo_log_

void acinfo_log_(int level,const struct ac_info *acinfo,const char * xstr)
{
	char str[8192];
	acinfo_print(str,acinfo);
	cw_dbg(DBG_CW_INFO,"%s",str);
//	cw_log_debug(level,"%s\n%s",xstr,str);
	return;
}




int join_state(struct conn * conn)
{
	struct wtpinfo * wtpinfo = get_wtpinfo();

	struct radioinfo ri;
	memset(&ri,0,sizeof(ri));
	int rc;


	struct radioinfo *rip = &(wtpinfo->radioinfo[0]);

//#ifdef WITH_CW_LOG_DEBUG	
//	char str[64];
//	sock_addrtostr(&conn->addr,str,64);
//	cw_log_debug0("Sending join request to %s",str);
//#endif	
	printf("Seqnum before = %i\n",conn->seqnum);
	rc = cwsend_join_request(conn,rip,wtpinfo);
	printf("Seqnum after = %i\n",conn->seqnum);

	
	struct cwrmsg * cwrmsg;
//	do {
int i;
for(int i=0; i<10; i++){
		
		cwrmsg = conn_get_message(conn);
		if ( cwrmsg)
			break;
}
	printf("Received %08p\n",cwrmsg);
//	}while(cwrmsg==0);


if(!cwrmsg) {
	exit(0);
}
	
	printf("SEQS %d %d\n",cwrmsg->type,conn->seqnum);

//	cw_log_debug0("Received message %i",cwrmsg->seqnum);

	if (cwrmsg->type != CWMSG_JOIN_RESPONSE || cwrmsg->seqnum != conn->seqnum){
		printf("Wrong message %d %d\n",cwrmsg->type,conn->seqnum);
		
	}

	struct ac_info acinfo;
	memset(&acinfo,0,sizeof(acinfo));

acinfo.result_code=99;

	cwread_join_response(&acinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);

	acinfo_log(0,&acinfo,"Connectet to the following AC");

	printf("Result_code: %d\n",acinfo.result_code);

}

*/





int run_join_d(struct conn * conn, struct sockaddr *sa)
{
	char addrstr[SOCK_ADDR_BUFSIZE];
	int sockfd;
	
	int rsec,lsec;
	int rc;

	/* Check if we support the same auth methods as the AC */
	lsec = cw_ktv_get_byte(conn->local_cfg,"ac-descriptor/security",0);
	rsec = cw_ktv_get_byte(conn->remote_cfg,"ac-descriptor/security",0);
	if ((lsec & rsec) == 0){
		cw_log(LOG_ERR, "Can't establish DTLS with AC, my sec: %d, remote sec %d",lsec,rsec);
		return 0;
	}

	/* setup a socket */
	conn->capwap_transition = CAPWAP_STATE_JOIN;
	sockfd = socket(sa->sa_family, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		cw_log(LOG_ERR, "Can't create socket: %s\n", strerror(errno));
		return -1;
	}
	sock_set_recvtimeout(sockfd, 1);
	conn->sock = sockfd;
	sock_copyaddr(&conn->addr, sa);


	/* we call connect to bind this socket to a local IP address,
	 * which we can later obtain by getsockname */
	rc = connect(sockfd, (struct sockaddr *) sa,
		     sock_addrlen((struct sockaddr *) sa));

	if (rc < 0) {
		cw_log(LOG_ERR, "Can't connect to %s: %s\n", 
			sock_addr2str(sa,addrstr),strerror(errno));
		close(sockfd);
		return -1;
	}


	cw_dbg(DBG_DTLS, "Establishing DTLS session with %s", sock_addr2str(sa, addrstr));

	rc = dtls_connect(conn);
	if (rc != 1) {
		dtls_shutdown(conn);
		cw_log(LOG_ERR, "Can't establish DTLS connection with %s",
		       sock_addr2str(sa,addrstr));
		close(sockfd);
		return 0;
	}


	cw_dbg(DBG_DTLS, "DTLS Connection successful established with %s",
	       sock_addr2str(sa,addrstr));

	run_join(conn);
	return 1;
}






int run_join(struct conn *conn)
{
	char addrstr[SOCK_ADDR_BUFSIZE];
/*	
//      cw_init_request(conn, CW_MSG_JOIN_REQUEST);
//      if ( cw_put_msg(conn, conn->req_buffer) == -1 )
//              return 0;
//
//      conn_send_msg(conn, conn->req_buffer);
*/

/*	mbag_del_all(conn->incomming);*/

/*	//mbag_del (conn->incomming,CW_ITEM_RESULT_CODE);*/

	int rc = cw_send_request(conn, CAPWAP_MSG_JOIN_REQUEST);

	if (!cw_result_is_ok(rc)) {
		if (rc > 0) {
			cw_log(LOG_ERR, "Can't Join AC at %s, AC said: %d - %s.",
			       sock_addr2str(&conn->addr,addrstr), rc, cw_strerror(rc));

		} else {
			cw_log(LOG_ERR, "Can't Join AC at %s: %d - %s.",
			       sock_addr2str(&conn->addr,addrstr), errno, cw_strerror(rc));
		}
		return 0;
	}

	cw_dbg(DBG_ELEM_IN, "Joined AC at %s,  Join Result: %d - %s",
	       sock_addr2str(&conn->addr,addrstr), rc, cw_strresult(rc));

	return 1;
}

int join(struct conn * conn, struct cw_DiscoveryResult * dis)
{
	mavliter_t ii;
	mavliter_init(&ii,dis->prio_ip);

	mavliter_foreach(&ii){
		int rc;
		cw_KTV_t * val,*ac;
		mavl_t rcfg;
		char * rk;
		char ipstr[100];
		char ac_name[CAPWAP_MAX_AC_NAME_LEN];
		struct sockaddr_storage sockaddr;
		
		val = mavliter_get(&ii);
		rk = val->key;
		val = val->val.ptr;
		val->type->to_str(val, ipstr, 100);
		
		rcfg = cw_ktv_get_sysptr(dis->prio_ac,rk,NULL);
		
		ac=cw_ktv_get(rcfg,"ac-name",CW_TYPE_BSTR16);
		if (ac != NULL){
			ac->type->to_str(ac,ac_name,sizeof(ac_name));
		}
		else{
			strcpy(ac_name,"");
		}
		
		
		cw_dbg(DBG_INFO, "Going to join CAPWAP controller '%s' at %s.",ac_name,ipstr);
		
		conn->remote_cfg=rcfg;

		sock_strtoaddr(ipstr,(struct sockaddr*)(&sockaddr));
		sock_setport((struct sockaddr*)&sockaddr,5246);
		rc = run_join_d(conn,(struct sockaddr*)(&sockaddr));
		if (rc)
			return 1;

	}
	return 0;
}
