
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
#include "cw/aciplist.h"
#include "cw/capwap_items.h"
#include "cw/mbag.h"

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

int run_join_d(struct sockaddr *sa)
{
	struct conn *conn = get_conn();
	conn->capwap_state = CW_STATE_JOIN;

	int sockfd;
	int rc;

	sockfd = socket(sa->sa_family, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		cw_log(LOG_ERR, "Can't create socket: %s\n", strerror(errno));
		return -1;
	}
	sock_set_recvtimeout(sockfd, 15);
	conn->sock = sockfd;
	sock_copyaddr(&conn->addr, sa);


	/* we call connect to bind this socket to a local IP address,
	 * which we can later obtain by getsockname */
/*	rc = connect(sockfd, (struct sockaddr *) sa,
		     sock_addrlen((struct sockaddr *) sa));

	if (rc < 0) {
		cw_log(LOG_ERR, "Can't connect to %s: %s\n", sock_addr2str(sa),
		       strerror(errno));
		close(sockfd);
		return -1;
	}
*/

	cw_dbg(DBG_DTLS, "Establishing DTLS session with %s", sock_addr2str(sa));

	int dtls_conf_ok=0;

	if (conf_dtls_psk) {
		conn->dtls_psk = conf_dtls_psk;
		conn->dtls_psk_len = strlen(conn->dtls_psk);
		conn->dtls_cipher = conf_dtls_cipher;
		dtls_conf_ok=1;
	}

	if (conf_sslkeyfilename && conf_sslcertfilename) {

		conn->dtls_key_file = conf_sslkeyfilename;
		conn->dtls_cert_file = conf_sslcertfilename;
		conn->dtls_key_pass = conf_sslkeypass;
		conn->dtls_cipher = conf_dtls_cipher;
		dtls_conf_ok=1;
	}

	if (!dtls_conf_ok){
		cw_log(LOG_ERR,"Can't establish DTLS connection with %s, neither psk nor cert set in config",
			sock_addr2str(sa));
		close(sockfd);
		return 0;
	}


	rc = dtls_connect(conn);
	if (rc != 1) {
		dtls_shutdown(conn);
		cw_log(LOG_ERR, "Can't establish DTLS connection with %s",
		       sock_addr2str(sa));
		close(sockfd);
		return 0;
	}


	cw_dbg(DBG_DTLS, "DTLS Connection successful established with %s",
	       sock_addr2str(sa));



	return 1;
}


int run_join(struct conn *conn)
{

//      cw_init_request(conn, CW_MSG_JOIN_REQUEST);
//      if ( cw_put_msg(conn, conn->req_buffer) == -1 )
//              return 0;
//
//      conn_send_msg(conn, conn->req_buffer);


	mbag_del_all(conn->incomming);

	//mbag_del (conn->incomming,CW_ITEM_RESULT_CODE);

	int rc = cw_send_request(conn, CW_MSG_JOIN_REQUEST);

	if (!cw_rcok(rc)) {
		if (rc > 0) {
			cw_log(LOG_ERR, "Can't Join AC at %s, AC said: %d - %s.",
			       sock_addr2str(&conn->addr), rc, cw_strerror(rc));

		} else {
			cw_log(LOG_ERR, "Can't Join AC at %s: %d - %s.",
			       sock_addr2str(&conn->addr), errno, cw_strerror(rc));
		}
		return 0;
	}

	cw_dbg(DBG_ELEM, "Joined AC at %s,  Join Result: %d - %s",
	       sock_addr2str(&conn->addr), rc, cw_strresult(rc));

	return 1;
}



int join()
{
	struct conn *conn = get_conn();

	printf("Join\n");

	mbag_del_all(conn->incomming);

	cw_aciplist_t iplist =
	    mbag_get_mavl(conn->local, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST);
	if (!iplist) {
		cw_log(LOG_ERR, "No IPs to join controller.");
		return 0;
	}

	if (!mavl_get_count(iplist)){
		cw_log(LOG_ERR, "No IPs to join controller. IP list is empty.");
		return 0;
	}


	DEFINE_AVLITER(ii, iplist);
	avliter_foreach(&ii) {

		cw_acip_t *ip = avliter_get(&ii);


		cw_dbg(DBG_INFO, "Going to join CAWAP controller on %s",
		       sock_addr2str_p(&ip->ip));

		int rc = run_join_d((struct sockaddr *) &ip->ip);

		if (rc<=0)
			continue;

		rc = run_join(conn);
		if (rc) {
			conn->capwap_state = CW_STATE_CONFIGURE;
			return 1;
		}



	}


	return 0;

}
