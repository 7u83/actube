#include <string.h>
#include <stdlib.h>


#include "stravltree.h"


static int cmp(const void *v1,const void*v2)
{
	return strcmp( (char*)v1,(char*)v2);
}

static void del(void* d)
{
	free (d);
	return;
}

struct avltree * stravltree_create()
{
	return avltree_create(cmp,del);	
}

const char * stravltree_add(struct avltree * t, const char * str)
{
	char * s = strdup(str);
	if (!s)
		return 0;
	return avltree_add(t,s);
}
