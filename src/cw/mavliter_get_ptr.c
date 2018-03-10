#include "mavl.h"

/**
 * Get the element, where AVL Iterator currently is positioned.
 * @param i AVL Iterator
 * @return element or NULL if not found.
 */
void * mavliter_get_ptr(mavliter_t *i)
{
	/*void * data;*/
	if (!i->cur)
		return NULL;

	return *(void**)(mavlnode_dataptr(i->cur));
}
