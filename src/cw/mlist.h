/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __MLIST_H
#define __MLIST_H

/**
 * @file
 * @brief Mlist Mini list
 */

#include <stdlib.h>
#include <stdint.h>


/**
 * @defgroup MLIST MLIST
 * @{
 */

/**
 * Element of an mlist. 
 * It's a simple connected list, just with pone connection to the 
 * next element. 
 */
struct mlistelem {
	struct mlistelem *next;
	struct mlistelem *prev;
};
typedef struct mlistelem mlistelem_t;

struct mlist {
/*	void *data;*/
	int (*cmp) (const void *d1, const void *d2);
	void (*del) (void *data);
	struct mlistelem *first;
	struct mlistelem *last;
	int count;
	size_t data_size;
};
typedef struct mlist * mlist_t;

#define mlistelem_dataptr(elem) ((void*)(((uint8_t*)(elem))+sizeof(struct mlistelem)))

/**
 * defgroup MLIST_FUNCTIONS
 * @{
 */
mlist_t mlist_create(int (*cmp) (const void *v1, const void *v2), void (*del)(void *), size_t data_size);

struct mlistelem *mlist_append(mlist_t l, void *data);
void mlist_destroy(mlist_t l);

extern struct mlistelem *mlist_find(mlist_t l, struct mlistelem *start, void *data);
extern struct mlistelem *mlist_replace(mlist_t l, struct mlistelem *start, void *data);



#define mlist_add mlist_append
/**
 * @}
 */

#define mlist_foreach(elem,list)\
	for (elem=list->first; elem; elem=elem->next)
		
		
#define mlistelem_get_ptr(elem) (*((void**)(mlistelem_dataptr(elem))))
#define mlistelem_get_str(elem) mlistelem_get_ptr(elem)
#define mlist_create_conststr() mlist_create(NULL,NULL,sizeof(const char*))
mlistelem_t * mlist_append_ptr (mlist_t list, void * ptr);



/**
 * @}
 */

#endif
