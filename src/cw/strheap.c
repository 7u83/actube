
#include "strheap.h"


static int cmp(const void *v1,const void*v2)
{
	return ((struct cw_StrListElem *)v1)->id - ((struct cw_StrListElem *)v2)->id;
}

static void del(void* d)
{
	return;
}

cw_strheap_t cw_strheap_create()
{
	return mavl_create(cmp,del,1312);	
}

const char * cw_strheap_add(cw_strheap_t t, struct cw_StrListElem *s)
{
	mavl_del(t,s);
	return mavl_add(t,s,NULL);
}

int cw_strheap_register_strings(cw_strheap_t h, struct cw_StrListElem *s) 
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


const char * cw_strheap_get(cw_strheap_t h, int id) {
	struct cw_StrListElem *r;
	struct cw_StrListElem s;
	s.id=id;
	r = mavl_get(h,&s);
	if (r) 
		return r->str;
	return NULL;
}


