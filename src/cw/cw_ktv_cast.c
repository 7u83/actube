#include "val.h"

cw_Val_t * cw_ktv_cast(cw_Val_t *v,const cw_Type_t * type)
{
	if (strcmp(v->type->name,type->name)==0)
		return v;
	if (type->cast==NULL)
		return NULL;
	if (!type->cast(v))
		return NULL;
		
	return v;
}

