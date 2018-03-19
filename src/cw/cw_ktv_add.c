#include "ktv.h"
#include "cw.h"

#include "log.h"
#include "dbg.h"

cw_KTV_t * cw_ktv_add(mavl_t kvtstore, const char *key, const struct cw_Type *type,
			const uint8_t * data, int len)
{
	cw_KTV_t mdata, *mresult;
	int exists;

/*	cw_dbg(DBG_ELEM,"KVStore (%p,%d) add elem (%s): %s", kvstore, kvstore->count,
			type->name, key );
*/
	mdata.key=cw_strdup(key);
	if (!mdata.key){
		cw_log(LOG_ERR, "Can't allocate memory for KTV key %s: %s",
				key,strerror(errno));
		return NULL;
	}

	mresult = type->get(&mdata,data,len);
	if (!mresult){
		cw_log(LOG_ERR, "Can't create kvstore element for key %s of type %s: %s",
				key,type->name, strerror(errno));
		free(mdata.key);
		return NULL;
	}
	
	mresult = mavl_add(kvtstore, &mdata, &exists);
	if (exists){
		cw_log(LOG_ERR, "Element already exists %s", key);
		/* element already exists */
		free(mdata.key);
		if (type->del)
			type->del(&mdata);
		return mresult;
	}

	return mresult;
}
