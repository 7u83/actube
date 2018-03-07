
#include <stdlib.h>

#include "cw_types.h"
#include "kvstore.h"

void cw_types_kvstore_mavl_delete(mavldata_t *data){
	const struct cw_Type * type = data->kv.priv;
	if (type->del)
		type->del(data);
	free(data->kv.key);
}

