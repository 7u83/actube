
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

//#include <iwlib.h>

#include "capwap/sock.h"
#include "capwap/conn.h"
#include "capwap/dtls.h"
#include "capwap/avltree.h"
#include "capwap/cw_log.h"
#include "capwap/capwap.h"
#include "capwap/cw_util.h"
#include "capwap/wtpinfo.h"

#include "wtp.h"
#include "wtp_conf.h"



struct wtpinfo wtpinfo;

//int conf_rids[2];
//int conf_rids_len[2];

//int responses = 0;

struct avltree * aclist =0;


extern struct wpa_driver_ops *wpa_drivers[];


#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>

//struct sockaddr conf_preferred_ac;


 
static void * drv_priv[2];
static void * drv_glob[2];

const char * interfaces[]={
	"wlan0",
	"wlan1-1"
};

/*static int drv_init()
{
	return 1;
}
*/

int main()
{
	wtp_main();
}


#include <time.h>



//#include <openssl/ssl.h>

int do_connect(void *priv,void *data)
{
	ACIP * ip = (ACIP*)data;
//	char str[100];
//	sock_addrtostr((struct sockaddr*)&ip->ip,str,100);

	sock_setport(&ip->ip,atoi(conf_control_port));
//	printf("Would connect to %s\n",str);
	
	int rc;
	rc = join(&ip->ip);
	if (!rc)
		return 1;

printf("Sleep after oin\n");
printf("Go conf\n");
	extern struct conn * get_conn();
	extern join_state(struct conn * conn);
	struct conn * conn = get_conn();
printf("Join conn = %p\n",conn);

//	join_state(conn);


//	rc = join(&ip->ip);
//	if (!rc)
//		return 1;

	rc = configure();

	run (get_conn());
	return 0;
}


#include "capwap/bstr.h"

int wtp_main(const char *ad)
{
	wtpconf_preinit();

	if (!read_config("./wtp_uci.conf")){
		return 1;
	}

	cw_dbg_opt_level = conf_dbg_level;

	wtpconf_init();
	
	


	

//	cw_log_debug_level=6; //conf_debug_level;

//	conf_dtls_psk="Tube";
//	conf_sslkeyfilename="../../ssl/wtp.key";
//	conf_sslcertfilename="../../ssl/wtp.crt";
//	conf_dtls_psk=0;

//	conf_sslkeyfilename="../../ssl/ac_cisco.key";
//	conf_sslcertfilename="../../ssl/ac_cisco.pem";
	conf_sslkeypass="Tube";



#ifdef WITH_DTLS	
	dtls_init();
#endif	
	
	//drv_init();


	while (1){
		ACIPLIST * aciplist=0;
		int i;

		cw_dbg(DBG_CW_INFO,"Entering discovery state");

		do {
			for (i=0; i<conf_ac_list_len; i++){
				if ((aciplist = do_discovery(conf_ac_list[i])))
					break;
			}
		}while (!aciplist);		

//		cw_log_debug0("Entering join state");
		if (!aciplist){
//			cw_log_debug0("Don't got any discovery response");
			exit(0);
		}

		aciplist_foreach(aciplist,do_connect,0);

		aciplist_destroy(aciplist);


	}
	exit(0);


	return 0;
}
