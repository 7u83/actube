
#include <errno.h>

#include "mavl.h"

#include "dbg.h"
#include "log.h"

#include "msgset.h"
#include "ktv.h"

static int cmp_cw_elemhandler_by_id(const void *elem1, const void *elem2)
{
	const struct cw_ElemHandler *e1 = elem1;
	const struct cw_ElemHandler *e2 = elem2;
	int r;
	r = e1->id - e2->id;
	if (r != 0)
		return r;
	r = e1->vendor - e2->vendor;
	if (r != 0)
		return r;
	r = e1->proto - e2->proto;
	if (r != 0)
		return r;
	return 0;
}

static int cmp_cw_elemhandler_by_key(const void *elem1, const void *elem2)
{
	const struct cw_ElemHandler *e1 = elem1;
	const struct cw_ElemHandler *e2 = elem2;
	return strcmp(e1->key, e2->key);
}

static int cmp_msgdata(const void *elem1, const void *elem2)
{
	const struct cw_MsgData *e1 = elem1;
	const struct cw_MsgData *e2 = elem2;
	return e1->type - e2->type;
}

static int cmp_elemdata(const void *elem1, const void *elem2)
{
	const struct cw_ElemData *e1 = elem1;
	const struct cw_ElemData *e2 = elem2;
	int r;
	r = e1->id - e2->id;
	if (r != 0)
		return r;
	r = e1->vendor - e2->vendor;
	if (r != 0)
		return r;
	r = e1->proto - e2->proto;
	if (r != 0)
		return r;
	return 0;

}

static void msgdata_destroy(struct cw_MsgData *data)
{
	if (!data)
		return;
	if (data->elements_list)
		mlist_destroy(data->elements_list);

	if (data->elements_tree)
		mavl_destroy(data->elements_tree);
	free(data);
}


static void del_mavl_msdgdata(void *d)
{
	msgdata_destroy(d);
}


/**
 * @brief Destroy a message set
 * @param set Message set to destroy
 */
void cw_msgset_destroy(struct cw_MsgSet *set)
{
	if (set->msgdata)
		mavl_destroy(set->msgdata);
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
struct cw_MsgSet *cw_msgset_create()
{

	/* allocate memory for a message_set */
	struct cw_MsgSet *set = malloc(sizeof(struct cw_MsgSet));
	if (set == NULL)
		return NULL;

	memset(set, 0, sizeof(struct cw_MsgSet));

	/* create mavl for all_elems by id */
	set->handlers_by_id = mavl_create(cmp_cw_elemhandler_by_id, NULL,
					  sizeof(struct cw_ElemHandler));
	if (set->handlers_by_id == NULL) {
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for all_elems by key */
	set->handlers_by_key = mavl_create(cmp_cw_elemhandler_by_key, NULL,
					   sizeof(struct cw_ElemHandler));
	if (set->handlers_by_key == NULL) {
		cw_msgset_destroy(set);
		return NULL;
	}

	/* create mavl for messages */
	set->msgdata = mavl_create(cmp_msgdata, del_mavl_msdgdata,
				   sizeof(struct cw_MsgData));
	if (set->msgdata == NULL) {
		cw_msgset_destroy(set);
		return NULL;
	}
	
	set->types_tree = cw_ktv_create_types_tree();
	if (set->types_tree == NULL){
		cw_msgset_destroy(set);
		return NULL;
	}

	return set;
}

struct cw_ElemHandler *cw_msgset_get_elemhandler(struct cw_MsgSet *set,
						 int proto, int vendor, int id)
{
	struct cw_ElemHandler search;
	search.proto = proto;
	search.vendor = vendor;
	search.id = id;
	return mavl_find(set->handlers_by_id, &search);
}


static int update_msgdata(struct cw_MsgSet *set, struct cw_MsgData *msgdata,
			  struct cw_MsgDef *msgdef)
{
	struct cw_ElemDef *elemdef;
	struct cw_ElemData ed, *result;
	mavliter_t it;

	/* iterate through all defined elements */
	for (elemdef = msgdef->elements; elemdef->id; elemdef++) {
		struct cw_ElemHandler *handler;
		int replaced;

		handler = cw_msgset_get_elemhandler(set,
						    elemdef->proto,
						    elemdef->vendor, elemdef->id);
		/* check if a handler for our element already exists */
		if (!handler) {
			cw_log(LOG_ERR, "Creating message set: No handler for message element: %d %d %d [%s]",
			       elemdef->proto, elemdef->vendor, elemdef->id, cw_strelem(elemdef->id));
			continue;
		}
		
/*		if (handler->type != NULL){
			if (mavl_add_ptr( set->types_tree, handler->type ) == NULL){
				cw_log(LOG_ERR, "Can't add type from handler: %s", strerror(errno));
				continue;
			}
		}
*/		

		ed.id = elemdef->id;
		ed.proto = elemdef->proto;
		ed.vendor = elemdef->vendor;
		ed.mand = elemdef->mand;

		/* add message element to the elements tree */
		result = mavl_replace(msgdata->elements_tree, &ed, &replaced);

		if (!replaced) {
			cw_dbg(DBG_MOD, "  adding message element %d %d %d - %s",
			       elemdef->proto,
			       elemdef->vendor, elemdef->id, handler->name);
		} else {
			cw_dbg(DBG_MOD, "  replacing message element %d %d %d - %s",
			       elemdef->proto,
			       elemdef->vendor, elemdef->id, handler->name);
		}
		
		/* add/delete/replace message elemeent to/from/in the elements list */
		switch ( elemdef->op & 0xff){
			case CW_IGNORE:
				break;
			case CW_DELETE:
				mlist_delete(msgdata->elements_list, &ed);
				break;
			case CW_APPEND:
				mlist_append(msgdata->elements_list, &ed);
				break;
			default:
			case CW_REPLACE:
				if (mlist_replace(msgdata->elements_list, &ed)==NULL){
					mlist_append(msgdata->elements_list, &ed);
				}
				break;
		}
	}


	if (msgdata->mand_keys!=NULL){
		mlist_destroy(msgdata->mand_keys);
	}
	msgdata->mand_keys = mlist_create_conststr();
	
	mavliter_init(&it,msgdata->elements_tree);
	mavliter_foreach(&it){
		struct cw_ElemHandler *handler;
		result = mavliter_get(&it);

		handler = cw_msgset_get_elemhandler(set,
						    result->proto,
						    result->vendor, result->id);
		if (result->mand){
			mlist_append_ptr(msgdata->mand_keys,(void*)handler->key);
			cw_dbg(DBG_MOD,"    Add mandatory key: %s",handler->key);
		}
		/*//printf("Have Result %d %d - %s\n",result->id,result->mand, handler->key);*/
	}
	

	return 0;
}


int cw_msgset_add(struct cw_MsgSet *set,
		  struct cw_MsgDef messages[], struct cw_ElemHandler handlers[]
    )
{

	struct cw_ElemHandler *handler;
	struct cw_MsgDef *msgdef;

	/* Create mavl for all handlers */
	for (handler = handlers; handler->id; handler++) {
		cw_dbg(DBG_MOD, "Adding handler for element %d - %s - with key: %s",
		       handler->id, handler->name, handler->key);
		mavl_replace(set->handlers_by_id, handler, NULL);
		mavl_replace(set->handlers_by_key, handler, NULL);
	}

	for (msgdef = messages; msgdef->type != 0; msgdef++) {
		struct cw_MsgData search;
		struct cw_MsgData *msg;
		int exists;

		/* add the message */
		search.type = msgdef->type;
		msg = mavl_add(set->msgdata, &search, &exists);
		if (msg == NULL) {
			cw_log(LOG_ERR, "Can't create messae");
			return 0;
		}

		/* Look if message already exists */
		if (!exists) {
			/* message is fresh createt, initialize data */
			msg->elements_tree = mavl_create(cmp_elemdata, NULL,
							 sizeof(struct cw_ElemData));
			msg->mand_keys=NULL;
			msg->elements_list = mlist_create(cmp_elemdata,NULL,sizeof(struct cw_ElemData));
			msg->postprocess=NULL;
			msg->preprocess=NULL;
		}

		/* Overwrite the found message */
		if (msgdef->name != NULL)
			msg->name = msgdef->name;
		if (msgdef->states != NULL)
			msg->states = msgdef->states;
		if (msgdef->postprocess != NULL)
			msg->postprocess = msgdef->postprocess;
		if (msgdef->preprocess != NULL)
			msg->preprocess = msgdef->preprocess;
		if (msgdef->next_state)
			msg->next_state=msgdef->next_state;
			
		msg->receiver = msgdef->receiver;


		cw_dbg(DBG_MOD, "Add message Type:%d - %s ", msgdef->type, msgdef->name);


		update_msgdata(set, msg, msgdef);
	}
	
	{
		mavliter_t it;
		cw_dbg(DBG_MOD,"  Known types:");
		mavliter_init(&it,set->types_tree);
		mavliter_foreach(&it){
			struct cw_Type * t = mavliter_get_ptr(&it);
			cw_dbg(DBG_MOD, "   Type: %s", t->name);
		}
	}
	
	

	return 0;
}

/**
 * @brief Find message data to a specific message
 * @param set message set
 * @param type message type to search for
 * @return message data or NULL if not found
 */
struct cw_MsgData *cw_msgset_get_msgdata(struct cw_MsgSet *set, int type)
{
	struct cw_MsgData search;
	search.type = type;
	return mavl_find(set->msgdata, &search);
}
