
#include "mavl.h"

int mavl_cmp_str(const union mavldata *e1, const union mavldata *e2){
	return strcmp (e1->str,e2->str);
}
