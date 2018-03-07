#ifndef __CW_KVSTORE_H
#define __CW_KVSTORE_H

#include "mavl.h"
#include "cw_types.h"

void cw_kvstore_mavl_delete(mavldata_t *data);
const char * cw_kvstore_add(mavl_t kvstore, const char *key, const struct cw_Type *type,
			const uint8_t * data, int len);
			
#define cw_kvstore_create()\
	mavl_create(mavl_cmp_kv, cw_kvstore_mavl_delete)


#endif /* __CW_KVSTORE_H */
