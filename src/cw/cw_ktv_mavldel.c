
#include <stdlib.h>


#include "val.h"

void cw_ktv_mavldel(void *data)
{
	struct cw_Val *ktv = data;
	if (ktv->type->del) {
		ktv->type->del(data);
	}
	free(ktv->key);
}
