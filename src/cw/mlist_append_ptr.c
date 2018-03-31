#include "mlist.h"

/**
 * @brief Append a pointer value to mlist
 * @param list List to append to
 * @param ptr Pointer to append
 * @return the created element or NULL if an error has occured
 */
mlistelem_t * mlist_append_ptr (mlist_t list, void * ptr)
{
	return mlist_append (list, &ptr);
}
