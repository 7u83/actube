

#include <stdlib.h>

#include "intavltree.h"


static int cmp(const void *v1,const void*v2)
{
	return  *((int*)v1) - *((char*)v2);
}

static void del(void* d)
{
	free (d);
	return;
}

struct avltree * intavltree_create()
{
	return avltree_create(cmp,del);	
}

int * intavltree_add(struct avltree * t, int val)
{
	int *v = avltree_get(t,&val);
	if (v) 
		return v;
	
	v = malloc(sizeof(int));
	if (!v)
		return NULL;
	*v=val;
	return avltree_add(t,v);
}
