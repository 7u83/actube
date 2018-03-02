#include "mavl.h"

void mavl_destroy(struct mavl *t)
{
	mavl_del_all(t);
	free (t);
}


