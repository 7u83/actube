
#include "mavl.h"

void mavl_free_bin(union mavldata *data){
	free(data->ptr);
}

