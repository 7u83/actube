#include "cw_types.h"

void cw_types_mavl_delete(mavldata_t *data){
	const struct cw_Type * type = data->kv.priv;
	if (type->del)
		type->del(data);
	free(data->kv.key);
}
