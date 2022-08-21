
#include "capwap.h"
#include "dbg.h"
#include "log.h"

int cw_check_missing_mand(struct cw_MsgData *msgdata, mavl_t keys, mavl_t handlers_by_key )
{
	mlistelem_t * elem;
	char *mandkey, *result;
	mlist_t missing;
	int count;

	if (msgdata==NULL)
		return 0;
	
	missing = mlist_create_conststr();
	if (missing==NULL){
		cw_log(LOG_ERR, "Can't allocate memory for check of missing mand elems: %s", strerror(errno));
		return 0;
	}

	mlist_foreach(elem, msgdata->mand_keys){
		mandkey = mlistelem_get_str(elem);
	
		result = mavl_get_str(keys,mandkey);
		if (result == NULL){
			mlist_append_ptr(missing,mandkey);
		}
	}
	
	mlist_foreach(elem,missing){
		const char * str;
		struct cw_ElemHandler search, *result;

		str = mlistelem_get_str(elem);

		search.key = str;
		result = mavl_get(handlers_by_key,&search);
		if (result == NULL){
			cw_log(LOG_ERR,"Can't find handler for for key %s",str);
		} 
		else {
			cw_dbg(DBG_RFC,"Missing mandatory message element: %s: %d - %s", 
					str,result->id, result->name);
		}


	}

	count = missing->count;
	mlist_destroy(missing);
	return count;
}



