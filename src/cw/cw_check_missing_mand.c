
#include "capwap.h"
#include "dbg.h"
#include "log.h"

int cw_check_missing_mand(struct cw_MsgData *msgdata, mavl_t keys )
{
	mlistelem_t * elem;
	char *mandkey, *result;
	mlist_t missing;
	int count;
	
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
		cw_dbg(DBG_RFC,"     Missing mandatory message element: %s", mlistelem_get_str(elem));
	}

	count = missing->count;
	mlist_destroy(missing);
	return count;
}



