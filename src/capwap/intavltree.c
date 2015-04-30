

#include <stdlib.h>

#include "intavltree.h"


static int cmp(const void *v1,const void*v2)
{
	return  *((int*)v1) - *((int*)v2);
}

static void del(void* d)
{
	free (d);
	return;
}

struct mavl * intavltree_create()
{
	return mavl_create(cmp,del);	
}

int * intavltree_add(struct mavl * t, int val)
{
	int *v = mavl_get(t,&val);
	if (v) 
		return v;
	
	v = malloc(sizeof(int));
	if (!v)
		return NULL;
	*v=val;
	return mavl_add(t,v);
}
