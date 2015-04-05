
/**
 * @file
 * @breif defines acinfo_print function
 */

#include <string.h>
#include <stdio.h>

#include "capwap.h"
#include "sock.h"

#include "acinfo.h"

#include "cw_util.h"


/**
 * Formats an acinfo object.
 *
 */ 
int acinfo_print(char *str,const struct ac_info *acinfo)
{
	char *s = str;

	s+=sprintf(s,"\tAC name: %s\n",acinfo->ac_name);

	s+=sprintf(s,"\tHardware version: ");
	s+=cw_format_version(s,acinfo->hardware_version,0,"-");
	s+=sprintf(s,"\n");

	s+=sprintf(s,"\tSoftware version: ");
	s+=cw_format_version(s,acinfo->software_version,0,"-");
	s+=sprintf(s,"\n");

	s+=sprintf(s,"\tStations: %i\n",acinfo->stations);
	s+=sprintf(s,"\tSation limit: %i\n",acinfo->limit);
	s+=sprintf(s,"\tActive WTPs: %i\n",acinfo->active_wtps);
	s+=sprintf(s,"\tMax WTPs: %i\n",acinfo->max_wtps);
	char help[64];
	sock_addrtostr((struct sockaddr*)&acinfo->local_ip,help,64);
	s+=sprintf(s,"\tLocal IP: %s\n",help);
	s+=sprintf(s,"\tECN support: %s\n",acinfo->ecn_support==0 ? "limited" : "full");
	s+=sprintf(s,"\tRMAC support: %s\n",acinfo->rmac==1 ? "supported" : "not supported");


	help[0]=0;
	if (acinfo->security & AC_SECURITY_S)
		strcpy(help,"psk");
	if (acinfo->security & AC_SECURITY_X){
		if (strlen(help))
			strcat(help,"/");
		strcat(help,"X.509");
	}
	if (!strlen(help))
		strcpy(help,"None");
	s+=sprintf(s,"\tSecurity: %s\n",help);

	help[0]=0;
/*	if (acinfo->dtls_policy & AC_DTLS_POLICY_D){
		strcpy(help,"dtls");
	}
	if (acinfo->dtls_policy & AC_DTLS_POLICY_C){
		if(strlen(help))
			strcat(help,"/");
		strcat(help,"clear");
	}
*/	if (!strlen(help))
		strcpy(help,"Not set");

	s+=sprintf(s,"\tDTLS policy: %s\n",help);

			
	return s-str;
}



