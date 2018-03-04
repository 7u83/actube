
#include "cw.h"
#include "mavl.h"
#include "dbg.h"

#include "message_set.h"

/*
typedef struct {
	int type;
	const char * name;
	int * states;
	mavl_t elements_tree;
	mlist_t elements_list;
}message2_t;
*/

static inline int cmp_cw_msgelemprops(const void *elem1, const void *elem2){
	cw_elem_handler_t * e1 = ((cw_msgelemprops_t*)elem1)->elem;
	cw_elem_handler_t * e2 = ((cw_msgelemprops_t*)elem2)->elem;
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
	message2_t * e1 = (message2_t*)elem1;
	message2_t * e2 = (message2_t*)elem2;
	
	return e1->type - e2->type;
}


void cw_message_set_destroy(cw_MsgSet_t * set){
	if (set->messages){
		mavl_destroy(set->messages);
	}
	if (set->all_elems){
		mavl_destroy(set->all_elems);
	}
	free(set);
}

cw_MsgSet_t * cw_message_set_create(){

	/* allocate memory for a message_set */
	cw_MsgSet_t  * set = malloc(sizeof(cw_MsgSet_t));
	if (set==NULL)
		return NULL;

	memset(set,0,sizeof(cw_MsgSet_t));

	/* create mavl for all_elems */
	set->all_elems = mavl_create(cmp_cw_msgelemprops,NULL);
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


static void update_message(message2_t * msg, cw_msgdef_t * src, cw_MsgSet_t * set){

	cw_msgelemprops_t *md;
	
	

	for (md = src->elements; md->elem!=0; md++){
		cw_dbg(DBG_INFO,"  add element %d - %s, %d",md->elem->id, md->elem->name, md->mand);
		mavl_add(msg->elements_tree,md);
		mavl_add(set->all_elems,md);
		mlist_append(msg->elements_list,md);
		mlist_replace(msg->elements_list,NULL,md);		
	}
	
}



void cw_msgset_add(cw_MsgSet_t * set,
			cw_msgdef_t  messages[]){

	cw_msgdef_t * message;
	for (message=messages; message->type !=0; message++){
		message2_t search, *next;
		
		search.type = message->type;

		cw_dbg(DBG_INFO,"Add message: Type:%d - %s",message->type,message->name);
		
		next = mavl_find(set->messages,&search);

		/** message not already in memory, 
		 * create a new one */
		if (next == NULL){
			next = malloc (sizeof(message2_t));
			if (next == NULL)
				return;
			next->elements_tree = mavl_create(cmp_cw_msgelemprops,NULL);
			if (next->elements_tree==NULL){
				free(next);
				return;
			}
			next->elements_list = mlist_create(cmp_cw_msgelemprops);
			if (next->elements_list == NULL){
				mavl_destroy(next->elements_tree);
				free(next);
				return;
			}
			next->type=message->type;
			mavl_add(set->messages,next);
			
		}
		/* massage is alreaddy in there */
		if (message->name)
			next->name=message->name;
		if (message->states)
			next->states=message->states;
		
		update_message(next,message, set);
	}
}

cw_elem_handler_t * cw_message_set_find_element(
			cw_MsgSet_t * set,
			cw_elem_handler_t * element){
	return mavl_find(set->all_elems,element);
}


mlist_t cw_msgset_get_msg(cw_MsgSet_t * set, int type){
	message2_t search;
	search.type = type;
	message2_t * result = mavl_find(set->messages,&search);
	if (!result){
		printf ("no result\n");
		return NULL;
		
	}
	return result->elements_list;
}
