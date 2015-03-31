
#include <malloc.h>
#include <string.h>

#include "cw_action.h"

static inline int cw_action_cmp(const void *elem1,const void *elem2)
{
	struct cw_action * e1 = (struct cw_action*)elem1;
	struct cw_action * e2 = (struct cw_action*)elem2;
	int r; 

	r = e1->elem_id - e2->elem_id;
	if (r!=0)
		return r;

	r = e1->msg_id - e2->msg_id;
	if (r!=0)
		return r;

	r = e1->capwap_state - e2->capwap_state;
	if (r!=0)
		return r;

	r = e1->vendor_id - e2->vendor_id;
	if (r!=0)
		return r;


	return 0;
}

/*
static void cw_action_del(void*d)
{
	free(d);
}
*/

cw_action_t * cw_actionlist_add(cw_actionlist_t t, struct cw_action * a)
{
	struct cw_action *an = malloc(sizeof(struct cw_action));
	if (!an)
		return NULL;

	memcpy(an,a,sizeof(struct cw_action));
	return avltree_add(t,an);
}


struct cw_action * cw_actionlist_get(cw_actionlist_t t,struct cw_action *a)
{
	return avltree_get(t,a);
}


cw_actionlist_t cw_actionlist_create()
{
	return avltree_create(cw_action_cmp,free); //cw_action_del);
}


int cw_register_actions(cw_actionlist_t t,cw_action_t * actions)
{
	while(actions->capwap_state){
//		cw_action_t *a = actions;
//		printf("State: %d MSG_ID: %d ELEM_ID: %d\n",a->capwap_state,a->msg_id,a->elem_id);
		cw_action_t * rc = cw_actionlist_add(t,actions);
		if (rc==0) 
			return 0;	
		actions++;
	}
	return 1;
}






