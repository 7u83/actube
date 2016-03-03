
#include "mavl.h"

static int cmp(const void *v1,const void*v2)
{
	return strcmp( (char*)v1,(char*)v2);
}

mavl_conststr_t mavl_create_conststr() 
{
	return mavl_create(cmp,NULL);
} 
