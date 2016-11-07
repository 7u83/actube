#include <stdlib.h>


#include "ac.h"
#include "conf.h"
#include "cw/capwap.h"
#include "cw/capwap_items.h"
#include "cw/aciplist.h"
#include "socklist.h"
#include "cw/sock.h"
#include "cw/dbg.h"

#include "wtpman.h"

struct cw_ac_status ac_status;


struct cw_actiondef capwap_actions;

mbag_t ac_config;


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
		if (acip->ip.ss_family == AF_INET)
			acip->index=11;
		else
			acip->index=3;
		

//		printf ("Adding IP %s\n",sock_addr2str(&acip->ip));	

		cw_aciplist_add(aciplist,acip);

		
	}
	return aciplist;
}












void * get_iplist(void * arg)
{
	return get_aciplist();
}

void release_iplist(void *arg,void *data)
{
}



/*
int handle_echo_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
		                      int len, struct sockaddr *from)
{
	cw_dbg(DBG_X,"Handle Echo Request %p",conn->data);
//	struct wtpman * wtpman = conn->data;
//	wtpman_echo_req(wtpman);
	return 0;
}

*/

/*static void setup_actions (struct mod_ac *c, struct mod_ac *b, struct cw_actiondef *actions)
{
	cw_dbg(DBG_X,"Setup Actions! Yea");
	cw_set_msg_end_callback(actions,CW_STATE_RUN,CW_MSG_ECHO_REQUEST,handle_echo_req);
}
*/

int ac_global_init()
{
//	mod_set_actions_registered_cb(setup_actions);



	ac_config = mbag_create();
	mbag_set_str(ac_config, CW_ITEM_AC_NAME, conf_acname);
	mbag_set_ptr(ac_config, CW_ITEM_AC_STATUS, &ac_status);

	ac_status.stations = 0;
	ac_status.limit = 1000;
	ac_status.active_wtps = 10;
	ac_status.max_wtps = 200;
	ac_status.security = CW_FLAG_AC_SECURITY_X | CW_FLAG_AC_SECURITY_S;
	ac_status.rmac_field = CW_FLAG_RMAC_SUPPORTED;
	ac_status.dtls_policy = CW_FLAG_DTLS_POLICY_C; // | CW_FLAG_DTLS_POLICY_D;


	mbag_set_bstrv(ac_config, CW_ITEM_AC_HARDWARE_VERSION, 0,
				 bstr_data(conf_hardware_version), bstr_len(conf_hardware_version));
	mbag_set_bstrv(ac_config, CW_ITEM_AC_SOFTWARE_VERSION, 0,
				 bstr_data(conf_software_version), bstr_len(conf_software_version));


	mbag_set_str(ac_config,CW_ITEM_AC_IMAGE_DIR,conf_image_dir);

//	mbag_set_avltree(ac_config, CW_ITEM_AC_IP_LIST, aciplist);

	
	mbag_set_fun(ac_config, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, get_iplist,release_iplist,(void*)771);

	return 1;
}
