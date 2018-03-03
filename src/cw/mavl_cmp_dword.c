#include "mavl.h"

int mavl_cmp_dword(const union mavldata *e1, const union mavldata *e2){
	return e1->dword - e2->dword;
}
