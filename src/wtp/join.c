
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "wtp_interface.h"
#include "wtp_conf.h"

#include "capwap/dtls.h"
#include "capwap/cw_log.h"
#include "capwap/sock.h"
#include "capwap/dtls.h"


#define acinfo_log acinfo_log_

void acinfo_log_(int level,const struct ac_info *acinfo,const char * xstr)
{
	char str[8192];
	acinfo_print(str,acinfo);
//	cw_log_debug(level,"%s\n%s",xstr,str);
	return;
}




int join_state(struct conn * conn)
{
	struct wtpinfo * wtpinfo = get_wtpinfo();

	struct radioinfo ri;
	memset(&ri,0,sizeof(ri));
	int rc;

#ifdef WITH_CW_LOG_DEBUG	
	char str[64];
	sock_addrtostr(&conn->addr,str,64);
//	cw_log_debug0("Sending join request to %s",str);
#endif	
	printf("Seqnum before = %i\n",conn->seqnum);
	rc = cwsend_join_request(conn,&ri,wtpinfo);
	printf("Seqnum after = %i\n",conn->seqnum);

	struct cwrmsg * cwrmsg = conn_get_message(conn);

	printf("Received %08p\n",cwrmsg);

//	cw_log_debug0("Received message %i",cwrmsg->seqnum);

	if (cwrmsg->type != CWMSG_JOIN_RESPONSE || cwrmsg->seqnum != conn->seqnum){
		printf("Wrong message\n");
	}

	struct ac_info acinfo;
	memset(&acinfo,0,sizeof(acinfo));

	cwread_join_response(&acinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
	acinfo_log(0,&acinfo,"Connectet to the following AC");
}



int join(struct sockaddr *sa)
{
	int sockfd;
	int rc;

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd==-1){
		cw_log(LOG_ERR,"Can't create socket: %s\n",strerror(errno));
		return -1;
	}

	sock_set_recvtimeout(sockfd,1);

	rc = connect(sockfd,(struct sockaddr*)sa,sock_addrlen((struct sockaddr*)sa));
	if (rc<0){
		char str[100];
		sock_addrtostr(sa,str,100);
		cw_log(LOG_ERR,"Can't connect to %s: %s\n",str,strerror(errno));
		close(sockfd);
		return -1;
	}

	struct conn * conn = get_conn();
	conn->sock=sockfd;
	sock_copyaddr(&conn->addr,sa);

	
#ifdef WITH_DTLS
	cw_dbg (DBG_DTLS,"Establishing DTLS session with %s",sock_addr2str(sa)); 



	if (conf_dtls_psk){
		conn->dtls_psk=conf_dtls_psk;
		conn->dtls_psk_len=strlen(conn->dtls_psk);
		conn->dtls_cipher=conf_dtls_cipher;
	}

	if (conf_sslkeyfilename && conf_sslcertfilename){

		conn->dtls_key_file = conf_sslkeyfilename;
		conn->dtls_cert_file = conf_sslcertfilename;
		conn->dtls_key_pass = conf_sslkeypass;
		conn->dtls_cipher=conf_dtls_cipher;
	
	}



	rc = dtls_connect(conn);
	if (rc!=1){
		dtls_shutdown(conn);
		char str[100];
		sock_addrtostr(sa,str,100);
		cw_log(LOG_ERR,"Can't establish DTLS connection to %s",str);
		close(sockfd);
		return 0;
	}


#endif	
	cw_dbg (DBG_DTLS,"DTLS session established with %s, cipher=%s",sock_addr2str(sa),dtls_get_cipher(conn)); 


	#ifdef WITH_CW_LOG_DEBUG
	{
		char str[100];
		sock_addrtostr(sa,str,100);
//		cw_log_debug0("DTLS connection to %s established",str);
	}
	#endif

	join_state(conn);

	return 1;
}
