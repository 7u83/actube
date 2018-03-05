#include "mavl.h"

int mavl_cmp_kv(const union mavldata *e1, const union mavldata *e2){
	return strcmp(e1->kv.key, e2->kv.key);
}
