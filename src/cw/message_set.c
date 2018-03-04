
#include <errno.h>

#include "mavl.h"
#include "debug.h"
#include "log.h"

#include "message_set.h"

static int cmp_cw_elemhandler_by_id(const mavldata_t * elem1, const mavldata_t *elem2){
	struct cw_ElemHandler * e1 = elem1->ptr;
	struct cw_ElemHandler * e2 = elem2->ptr;
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

static int cmp_cw_elemhandler_by_key(const mavldata_t *elem1, const mavldata_t *elem2){
	struct cw_ElemHandler * e1 = elem1->ptr;
	struct cw_ElemHandler * e2 = elem2->ptr;
	return strcmp(e1->key,e2->key);
}

static int cmp_msgdata(const mavldata_t * elem1, const mavldata_t *elem2)
{       
	struct cw_MsgData * e1 = elem1->ptr;
	struct cw_MsgData * e2 = elem2->ptr;
	return e1->type - e2->type;
}

static int cmp_elemdata(const mavldata_t * elem1, const mavldata_t *elem2)
{
	struct cw_ElemData * e1 = elem1->ptr;
	struct cw_ElemData * e2 = elem2->ptr;
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

static void msgdata_destroy(struct cw_MsgData *data){
	if (!data)
		return;
/*	if (data->elements_list)
		mlist_destroy(data->elements_list);
*/
	if (data->elements_tree)
		mavl_destroy( data->elements_tree );
	free(data);
}

static struct cw_MsgData * msgdata_create(int type){
	struct cw_MsgData * msg;
	
	msg = malloc( sizeof(struct cw_MsgData));
	if (!msg) 
		return NULL;

	msg->elements_tree = mavl_create(cmp_elemdata,mavl_free_bin);
	if (!msg->elements_tree){
		msgdata_destroy(msg);
		return NULL;
	}

	msg->type=type;
	return msg;
}


static void del_mavl_msdgdata( mavldata_t * d ){
	msgdata_destroy(d->ptr);
}


/**
 * @brief Destroy a message set
 * @param set Message set to destroy
 */
void cw_msgset_destroy(struct cw_MsgSet * set){
	if (set->messages)
		mavl_destroy(set->messages);
	if (set->handlers_by_id)
		mavl_destroy(set->handlers_by_id);
	if (set->handlers_by_key)
		mavl_destroy(set->handlers_by_key);
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
	set->handlers_by_id = mavl_create(cmp_cw_elemhandler_by_id,mavl_free_bin);
	if (set->handlers_by_id==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for all_elems by id */
	set->handlers_by_key = mavl_create(cmp_cw_elemhandler_by_key,NULL);
	if (set->handlers_by_key==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for messages */
	set->messages = mavl_create(cmp_msgdata,del_mavl_msdgdata);
	if (set->messages==NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	return set;
}

struct cw_ElemHandler * cw_msgset_get_elemhandler(struct cw_MsgSet * set,
		int proto, int vendor, int id)
{
	struct cw_ElemHandler search;
	search.proto=proto;
	search.vendor=vendor;
	search.id=id;
	return mavl_find_ptr(set->handlers_by_id,&search);
}


static int update_msgdata(struct cw_MsgSet * set, struct cw_MsgData * msgdata,
		struct cw_MsgDef * msgdef)
{
	struct cw_ElemDef * elemdef;
	
	/* iterate through all defined elements */
	for(elemdef = msgdef->elements; elemdef->id; elemdef++){
		struct cw_ElemHandler * handler;
		
		handler = cw_msgset_get_elemhandler(set,
			elemdef->proto,
			elemdef->vendor,
			elemdef->id);

		if (!handler){
			cw_log(LOG_ERR, "No handler for message element: %d %d %d",
					elemdef->proto,
					elemdef->vendor,
					elemdef->id);
			continue;
		}

		DBG_START(NULL,DBG_INFO)
			"Have an element %d %d %d, %s", 
				elemdef->proto,
				elemdef->id, 
				elemdef->vendor,
				handler->name
		DBG_END
	}
	
	return 0;
}


int cw_msgset_add(struct cw_MsgSet * set,
			struct cw_MsgDef messages[], 
			struct cw_ElemHandler handlers[]
			){

	struct cw_ElemHandler * handler;
	struct cw_MsgDef * msgdef;

	/* Create mavl for all handlers */
	for(handler = handlers; handler->id; handler++){
		mavldata_t copy;
		copy.ptr = malloc(sizeof(struct cw_ElemHandler));
		if (!copy.ptr){
			cw_log(LOG_ERR,"Can't alloc mem:", strerror(errno));
			continue;
		}
		
		DBG_START(NULL,DBG_MOD)
			"  Adding handler for element %d - %s - with key: %s", 
				handler->id,
				handler->name,
				handler->key
		DBG_END
		
		memcpy(copy.ptr,handler,sizeof(struct cw_ElemHandler));
		
		mavl_replace(set->handlers_by_id,&copy);
		mavl_replace(set->handlers_by_key,&copy);
	}


	for (msgdef=messages; msgdef->type !=0; msgdef++){
		struct cw_MsgData search, *msg;
		
		/* Look if message already exists */ 
		search.type=msgdef->type;
		msg = mavl_find_ptr(set->messages,&search);

		if (!msg){
			msg = msgdata_create(msgdef->type);
			
			if (!msg){
				cw_log(LOG_ERR,"Can't create messae");
				return 0;
			}

			mavl_add_ptr(set->messages,msg);
		}
	
		
		
		/* Overwrite the found message */
		if (msgdef->name)
			msg->name=msgdef->name;
		if (msgdef->states)
			msg->states=msgdef->states;
		msg->receiver=msgdef->receiver;

		DBG_START(NULL,DBG_INFO)
		   "Add message Type:%d - %s ",msgdef->type,msgdef->name
		DBG_END
	
		update_msgdata(set,msg,msgdef);
	}

	return 0;
}

/**
 * @brief Find message data to a specific message
 * @param set message set
 * @param type message type to search for
 * @return message data or NULL if not found
 */
struct cw_MsgData * cw_msgset_get_msgdata(struct cw_MsgSet *set,int type){
	struct cw_MsgData search;
	search.type=type;
	return mavl_find_ptr(set->messages,&search);
}
