#include "mavl.h"

/**
 * Get the element, where AVL Iterator currently is positioned.
 * @param i AVL Iterator
 * @return element or NULL if not found.
 */
union mavldata * mavliter_get(mavliter_t *i){
	if(!i->cur)
		return NULL;
	return &i->cur->data;	
}


