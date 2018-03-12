
#include "ktv.h"

int cw_ktv_mavlcmp_type_by_name (const void *v1, const void *v2)
{
	return strcmp ( (*((struct cw_Type**) v1))->name, 
			(*((struct cw_Type**) v2))->name);
}
