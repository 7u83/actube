#include "ktv.h"

cw_KTV_t * cw_ktv_cast(cw_KTV_t *v,const cw_Type_t * type)
{
	if (strcmp(v->type->name,type->name)==0)
		return v;
	if (type->cast==NULL)
		return NULL;
	if (!type->cast(v))
		return NULL;
		
	return v;
}

