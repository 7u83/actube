
#include "cw.h"
#include "mavl.h"

#include "message_set.h"

struct message_search_elem{
	cw_messagedef_t *message;
	mavl_t elements_tree;
	mlist_t elements_list;
};


static inline int cmp_cw_message_element(const void *elem1, const void *elem2){
	cw_message_element_t * e1 = (cw_message_element_t*)elem1;
	cw_message_element_t * e2 = (cw_message_element_t*)elem2;
	int r;
	r = e1->id - e2->id;
	if (r!=0)
		return r;
	r = e1->vendor - e2->vendor;
	if (r!=0)
		return r;
        r = e1->proto - e2->proto;
        if (r != 0)
                return r;
	return 0;
}


static inline int msg_cmp(const void *elem1, const void *elem2)
{       
/*        struct cw_action_in *e1 = (struct cw_action_in *) elem1;
        struct cw_action_in *e2 = (struct cw_action_in *) elem2;
        int r;

        r = e1->capwap_state - e2->capwap_state;
        if (r != 0)
                return r;

        r = e1->msg_id - e2->msg_id;
        if (r != 0)
                return r;

        r = e1->elem_id - e2->elem_id;
        if (r != 0)
                return r;

        r = e1->vendor_id - e2->vendor_id;
        if (r != 0)
                return r;

        r = e1->proto - e2->proto;
        if (r != 0)
                return r;
*/
        return 0;
}


void cw_message_set_destroy(cw_message_set_t * set){
	if (set->messages){
		mavl_destroy(set->messages);
	}
	if (set->all_elems){
		mavl_destroy(set->all_elems);
	}
	free(set);
}

cw_message_set_t * cw_message_set_create(){

	/* allocate memory for a message_set */
	cw_message_set_t  * set = malloc(sizeof(cw_message_set_t));
	if (set==NULL)
		return NULL;

	memset(set,0,sizeof(cw_message_set_t));

	/* create mavl for all_elems */
	set->all_elems = mavl_create(cmp_cw_message_element,NULL);
	if (set->all_elems==NULL){
		cw_message_set_destroy(set);
		return NULL;
	}

	/* create mavl for messages */
	set->messages = mavl_create(msg_cmp,NULL);
	if (set->messages==NULL){
		cw_message_set_destroy(set);
		return NULL;
	}

	return set;

}



void cw_message_set_add(cw_message_set_t * set,
			cw_message_t * messages, 
			cw_message_element_t * elements[]){
	
	cw_message_element_t ** e;
	for (e=elements; e->id!=0; e++){

	//	mavl_add(set->all_elems, e);

		printf("Add: %d %d %d %s\n",
			e->proto,
			e->vendor,
			e->id,
			e->name
		);
		
		
	}
}
