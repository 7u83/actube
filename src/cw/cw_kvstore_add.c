#include "kvstore.h"
#include "cw_types.h"
#include "cw.h"

#include "log.h"
#include "dbg.h"

const char * cw_kvstore_add(mavl_t kvstore, const char *key, const struct cw_Type *type,
			const uint8_t * data, int len)
{
	mavldata_t mdata, *mresult;

/*	cw_dbg(DBG_ELEM,"KVStore (%p,%d) add elem (%s): %s", kvstore, kvstore->count,
			type->name, key );
*/
	mdata.kv.key=cw_strdup(key);
	if (!mdata.kv.key){
		cw_log(LOG_ERR, "Can't allocate memory for key %s: %s",
				key,strerror(errno));
		return NULL;
	}


	mresult = type->get(&mdata,data,len);
	if (!mresult){
		cw_log(LOG_ERR, "Can't create kvstore element for key %s of type %s: %s",
				key,type->name, strerror(errno));
		free(mdata.kv.key);
		return NULL;
	}
	
	mresult = mavl_add(kvstore, &mdata);
	if (mresult != &mdata){
		cw_log(LOG_ERR, "Element already exists %s", key);
		/* element already exists */
		free(mdata.kv.key);
		type->del(&mdata);
		return key;
	}

	return mdata.kv.key;
}
