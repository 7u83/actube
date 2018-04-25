#include "ktv.h"
#include "cw.h"

#include "log.h"
#include "dbg.h"

cw_KTV_t * cw_ktv_add(mavl_t kvtstore, const char *key, const struct cw_Type *type, 
			const void * valguard,
			const uint8_t * data, int len)
{
	cw_KTV_t mdata, *mresult;
	int exists;

	mdata.key=cw_strdup(key);
	mdata.valguard=valguard;
	
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

	mavl_del(kvtstore,&mdata);
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
