#include "kvt.h"

int cw_kvt_mavlcmp(const void *v1, const void *v2)
{
	return strcmp(((cw_KVT_t *) v1)->key, ((cw_KVT_t *) v2)->key);
}
