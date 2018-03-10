
#include <stdlib.h>


#include "kvt.h"

void cw_kvt_mavldel(void *data)
{
	struct cw_KVT *kvt = data;

	if (kvt->type->del)
		kvt->type->del(data);

	free(kvt->key);
}
