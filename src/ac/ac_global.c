#include <stdlib.h>


#include "ac.h"
#include "conf.h"
#include "capwap/capwap.h"
#include "capwap/capwap_items.h"
#include "capwap/aciplist.h"
#include "socklist.h"
#include "capwap/sock.h"

struct cw_ac_status ac_status;


struct cw_actiondef capwap_actions;

cw_itemstore_t ac_config;


cw_aciplist_t get_aciplist()
{
	int i=0;

	cw_aciplist_t aciplist = cw_aciplist_create();

	if(!aciplist)
		return 0;


	for (i=0; i<socklist_len;i++){

		if (socklist[i].type != SOCKLIST_UNICAST_SOCKET)
			break;

		struct sockaddr_storage sa;
		unsigned int salen=sizeof(sa);
		if ( getsockname(socklist[i].sockfd,(struct sockaddr*)&sa,&salen)<0)
			continue;

		cw_acip_t * acip;
		acip = malloc(sizeof(cw_acip_t));
		if (!acip)
			continue;

		sock_copyaddr(&acip->ip,(struct sockaddr*)&sa);
		acip->wtp_count=13;
		

//		printf ("Adding IP %s\n",sock_addr2str(&acip->ip));	

		cw_aciplist_add(aciplist,acip);

		
	}
	return aciplist;
}












void * get_iplist(void * arg)
{
	printf("************************************************************************************\n");
	printf("The ist an iplist getter\n");
	return get_aciplist();
}

void release_iplist(void *arg,void *data)
{
	printf("And an releaser\n");
}

int ac_global_init()
{
	ac_config = cw_itemstore_create();
	cw_itemstore_set_str(ac_config, CW_ITEM_AC_NAME, conf_acname);
	cw_itemstore_set_ptr(ac_config, CW_ITEM_AC_STATUS, &ac_status);

	ac_status.stations = 0;
	ac_status.limit = 1000;
	ac_status.active_wtps = 10;
	ac_status.max_wtps = 200;
	ac_status.security = CW_FLAG_AC_SECURITY_X | CW_FLAG_AC_SECURITY_S;
	ac_status.rmac_field = CW_FLAG_RMAC_SUPPORTED;
	ac_status.dtls_policy = CW_FLAG_DTLS_POLICY_C | CW_FLAG_DTLS_POLICY_D;


	cw_itemstore_set_vendorstr(ac_config, CW_ITEM_AC_HARDWARE_VERSION, 0,
				 bstr_data(conf_hardware_version), bstr_len(conf_hardware_version));
	cw_itemstore_set_vendorstr(ac_config, CW_ITEM_AC_SOFTWARE_VERSION, 0,
				 bstr_data(conf_software_version), bstr_len(conf_software_version));


	cw_itemstore_set_str(ac_config,CW_ITEM_AC_IMAGE_DIR,conf_image_dir);

//	cw_itemstore_set_avltree(ac_config, CW_ITEM_AC_IP_LIST, aciplist);

	
	cw_itemstore_set_fun(ac_config, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, get_iplist,release_iplist,(void*)771);

	return 1;
}
