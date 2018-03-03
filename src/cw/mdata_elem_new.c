#include <stdlib.h>


#include "mdata.h"

struct mdata_Elem *mdata_elem_new( const struct mdata_Type * type )
{
	struct mdata_Elem *i= malloc(sizeof(struct mdata_Elem));
	if (!i)
		return NULL;

	i->type = type;
	return i;
}

