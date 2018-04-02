#include <stdlib.h>


#include "ac.h"
#include "conf.h"
#include "cw/capwap.h"

/*#include "cw/aciplist.h"*/
#include "socklist.h"
#include "cw/sock.h"
#include "cw/dbg.h"

#include "wtpman.h"
#include "discovery_cache.h"

mavl_t actube_global_cfg;



/*
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
		

/ * //		printf ("Adding IP %s\n",sock_addr2str(&acip->ip));	
* /
		cw_aciplist_add(aciplist,acip);

		
	}
	return aciplist;
}

*/









/*
void * get_iplist(void * arg)
{
	return get_aciplist();
}
*/

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


struct cw_DiscoveryCache * discovery_cache;

int ac_global_init()
{
	/* initialize discovery cache */
	discovery_cache = discovery_cache_create(100);
	if (discovery_cache==NULL)
		goto errX;


	return 1;
errX:
	return 0;
}
