#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "sock.h"

#include "socklist.h"
#include "acinfo.h"
#include "conf.h"

#include "capwap.h"

ACIPLIST * get_aciplist()
{
	int i=0;

	ACIPLIST * aciplist = aciplist_create();
	if(!aciplist)
		return 0;


	for (i=0; i<socklist_len;i++){

		if (socklist[i].type != SOCKLIST_UNICAST_SOCKET)
			break;

		struct sockaddr_storage sa;
		unsigned int salen=sizeof(sa);
		if ( getsockname(socklist[i].sockfd,(struct sockaddr*)&sa,&salen)<0)
			continue;

		ACIP * acip;
		acip = malloc(sizeof(ACIP));
		if (!acip)
			continue;

		sock_copyaddr(&acip->ip,(struct sockaddr*)&sa);
		acip->wtp_count=17;


		aciplist_add(aciplist,acip);

		
	}
	return aciplist;
}






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
	acinfo->rmac=2; /* radio mac not supported */

	acinfo->vendor_id=conf_vendor_id;
	acinfo->hardware_version=(uint8_t*)conf_hardware_version;
	acinfo->software_version=(uint8_t*)conf_software_version;

	if (conf_dtls_psk)
		acinfo->security|=AC_SECURITY_S;

	acinfo->dtls_policy = AC_DTLS_POLICY_C;
//	acinfo->ac_ips = conf_ac_ips;
//	acinfo->ac_ips_len=conf_ac_ips_len;

//	acinfo->salist=conf_salist;
//	acinfo->salist_len=conf_salist_len;

//	acinfo->salist = conf_ac_ips;
//	acinfo->salist_len = conf_ac_ips_len;

	acinfo->aciplist=get_aciplist();


	return acinfo;

}



get_join_result()
{
	return 0;
}

