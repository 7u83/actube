
#include "cw.h"
#include "mavl.h"
#include "cw/dbg.h"
#include "cw/log.h"

#include "message_set.h"

static int cmp_cw_elemhandler_by_id(const void *elem1, const void *elem2){
	struct cw_ElemHandler * e1 = ((struct cw_ElemHandler*)elem1);
	struct cw_ElemHandler * e2 = ((struct cw_ElemHandler*)elem2);
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

static int cmp_cw_elemhandler_by_key(const void *elem1, const void *elem2){
	struct cw_ElemHandler * e1 = ((struct cw_ElemHandler*)elem1);
	struct cw_ElemHandler * e2 = ((struct cw_ElemHandler*)elem2);
	return strcmp(e1->key,e2->key);
}


static int cmp_msgdata(const void *elem1, const void *elem2)
{       
	struct cw_MsgData * e1 = (struct cw_MsgData*)elem1;
	struct cw_MsgData * e2 = (struct cw_MsgData*)elem2;
	return e1->type - e2->type;
}

static void msgdata_destroy(struct cw_MsgData *data){
	if (!data)
		return;
	if (data->elements_list)
		mlist_destroy(data->elements_list);
	if (data->elements_tree)
		mavl_destroy( data->elements_tree );
	free(data);
}

static struct cw_MsgData * msgdata_create(){
	struct cw_MsgData * msg;
	msg = malloc( sizeof(struct cw_MsgData));
	if (!msg) 
		return NULL;

/*	msg->elements_tree = mavl_create();
	if (!msg->elements_tree){
		msgdata_destroy(msg);
		return NULL;
	}
*/
}



/**
 * @brief Destroy a message set
 * @param set Message set to destroy
 */
void cw_msgset_destroy(struct cw_MsgSet * set){
	if (set->messages)
		mavl_destroy(set->messages);
	if (set->all_elems_by_id)
		mavl_destroy(set->all_elems_by_id);
	if (set->all_elems_by_key)
		mavl_destroy(set->all_elems_by_key);
	free(set);
}

/**
 * @brief Create a message set
 * @return Message set create, NULL if an error has occured
 */
struct cw_MsgSet * cw_msgset_create(){

	/* allocate memory for a message_set */
	struct cw_MsgSet * set = malloc(sizeof(struct cw_MsgSet ));
	if (set==NULL)
		return NULL;

	memset(set,0,sizeof(struct cw_MsgSet));

	/* create mavl for all_elems by id */
	set->all_elems_by_id = mavl_create(cmp_cw_elemhandler_by_id,free);
	if (set->all_elems_by_id==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for all_elems by id */
	set->all_elems_by_key = mavl_create(cmp_cw_elemhandler_by_key,NULL);
	if (set->all_elems_by_key==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for messages */
	set->messages = mavl_create(cmp_msgdata,msgdata_destroy);
	if (set->messages==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	return set;
}





int cw_msgset_add(struct cw_MsgSet * set,
			struct cw_MsgDef messages[], 
			struct cw_ElemHandler elements[]
			){

	struct cw_ElemHandler * handler;
	struct cw_MsgDef * message;
	
	/* Create mavl for all handlers */
	for(handler = elements; handler->id; handler++){
		struct cw_ElemHandler * copy;
		copy = malloc(sizeof(struct cw_ElemHandler));
		if (!copy){
			cw_log(LOG_ERR,"Can't alloc mem:", strerror(errno));
			continue;
		}
		
		memcpy(copy,handler,sizeof(struct cw_ElemHandler));
		
		mavl_replace(set->all_elems_by_id,copy);
		mavl_replace(set->all_elems_by_key,copy);
	}


	for (message=messages; message->type !=0; message++){
		
		struct cw_MsgData * msg;
		
		cw_dbg(DBG_INFO,"Add message: Type:%d - %s",message->type,message->name);
		
		msg = malloc( sizeof(struct cw_MsgData));
		if (!msg) {
			cw_log(LOG_ERROR,"Can't alloc cw_MsgData: %s",strerror(errno));
			continue;
		}



	}
}

struct cw_ElemHandler * cw_message_set_find_element(
			struct cw_MsgSet * set,
			struct cw_ElemHandler * element){
	return mavl_find(set->all_elems_by_id,element);
}


mlist_t cw_msgset_get_msg(struct cw_MsgSet * set, int type){
	struct cw_MsgData search;
	search.type = type;
	struct cw_MsgData * result = mavl_find(set->messages,&search);
	if (!result){
		printf ("no result\n");
		return NULL;
		
	}
	return result->elements_list;
}
