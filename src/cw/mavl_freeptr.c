#include "mavl.h"

void mavl_freeptr(void *ptr){
	free( *((void**)ptr) );
}
