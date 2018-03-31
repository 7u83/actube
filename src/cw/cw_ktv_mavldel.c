
#include <stdlib.h>


#include "ktv.h"

void cw_ktv_mavldel(void *data)
{
	struct cw_KTV *ktv = data;
	if (ktv->type->del) {
		ktv->type->del(data);
	}
	free(ktv->key);
}
