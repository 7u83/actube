
#include <stdlib.h>


#include "ktv.h"

void cw_ktv_mavldel(void *data)
{
	struct cw_KTV *kvt = data;

	if (kvt->type->del)
		kvt->type->del(data);

	free(kvt->key);
}
