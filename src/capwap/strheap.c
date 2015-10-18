
#include "strheap.h"


static int cmp(const void *v1,const void*v2)
{
	return ((struct cw_strlist_elem *)v1)->id - ((struct cw_strlist_elem *)v2)->id;
}

static void del(void* d)
{
	return;
}

cw_strheap_t cw_strheap_create()
{
	return mavl_create(cmp,del);	
}

const char * cw_strheap_add(cw_strheap_t t, struct cw_strlist_elem *s)
{
	mavl_del(t,s);
	return mavl_add(t,s);
}

int cw_strheap_register_strings(cw_strheap_t h, struct cw_strlist_elem *s) 
{
	int n=0;
	while ( s->id!=CW_STR_STOP){
		cw_strheap_add(h,s);
		s++;
		n++;
	}
	cw_strheap_add(h,s);
	return n+1;
}

