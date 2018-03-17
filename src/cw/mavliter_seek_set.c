#include "mavl.h"

void * mavliter_seek_set ( struct mavliter *i )
{
	i->stack_ptr = 0;
	i->cur = i->root;
	return mavliter_next ( i );
}
