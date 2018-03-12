#include "ktv.h"

int cw_ktv_mavlcmp(const void *v1, const void *v2)
{
	return strcmp(((cw_KTV_t *) v1)->key, ((cw_KTV_t *) v2)->key);
}
