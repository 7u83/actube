#include "mavl.h"

union mavldata *mavl_add_kv_byte(mavl_t tree, const char *key, uint8_t val)
{
	mavldata_t data;
	data.kv.key = key;
	data.kv.val.byte = val;
	return mavl_add(tree, &data);
}
