#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "cw/sock.h"

#include "socklist.h"
#include "conf.h"

#include "cw/capwap.h"
//#include "cw/capwap_80211.h"
#include "cw/aciplist.h"


/*
int pr(void *x,void *y){
}
*/

/*
struct radioinfo * get_radioinfo()
{
	
}
*/


/*
struct ac_info * get_acinfo()
{
	
	struct ac_info * acinfo;
	acinfo = malloc(sizeof(struct ac_info));
	if(!acinfo)
		return 0;

	memset(acinfo,0,sizeof(struct ac_info));
	acinfo->ac_name=conf_acname;

	acinfo->stations=10;
	acinfo->limit=10000;
	acinfo->active_wtps=10;
	acinfo->max_wtps=conf_max_wtps;
	acinfo->rmac=1; 	

	acinfo->vendor_id=conf_vendor_id;

	acinfo->hardware_version=(uint8_t*)conf_hardware_version;
	acinfo->software_version=(uint8_t*)conf_software_version;
	acinfo->cisco_hardware_version=(uint8_t*)conf_cisco_hardware_version;
	acinfo->cisco_software_version=(uint8_t*)conf_cisco_software_version;


	if (conf_dtls_psk)
		acinfo->security|=AC_SECURITY_S;
	if (conf_sslkeyfilename && conf_sslcertfilename)
		acinfo->security|=AC_SECURITY_X;


//	acinfo->dtls_policy = AC_DTLS_POLICY_C | AC_DTLS_POLICY_D ;
//	acinfo->ac_ips = conf_ac_ips;
//	acinfo->ac_ips_len=conf_ac_ips_len;

//	acinfo->salist=conf_salist;
//	acinfo->salist_len=conf_salist_len;

//	acinfo->salist = conf_ac_ips;
//	acinfo->salist_len = conf_ac_ips_len;

	//acinfo->aciplist=get_aciplist();


	aciplist_foreach(acinfo->aciplist,pr,NULL);
*/

/*
	int i;
	for (i=1; i<=4; i++){
		acinfo->radioinfos[i].type= 
			CW_80211_RADIO_TYPE_B |
			CW_80211_RADIO_TYPE_A | 
			CW_80211_RADIO_TYPE_G | 
			CW_80211_RADIO_TYPE_N;
//		acinfo->radioinfos[i].type=0xffffffff; 
		acinfo->radioinfos[i].rid=i;
	}


	return acinfo;

}

*/

/*
get_join_result()
{
	return 0;
}
*/

