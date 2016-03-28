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

/**
 * @defgroup MLIST MLIST
 * @{
 */

/**
 * Element of an mlist. 
 * It's a simple connected list, just with pone connection to the 
 * next element. 
 */
struct mlist_elem {
	/** Pointer to data */
	void *data;
	/** 
	 * Pointer to next element 
	 * The last element is determined by a NULL pointer
	 * */
	struct mlist_elem *next;
};

struct mlist {
	void *data;
	int (*cmp) (void *d1, void *d2);
	struct mlist_elem *list;
};

typedef struct mlist mlist_t;


/**
 * defgroup MLIST_FUNCTIONS
 * @{
 */
extern mlist_t *mlist_create(int (*cmp) (void *v1, void *v2));
extern struct mlist_elem *mlist_append(mlist_t * l, void *data);
extern struct mlist_elem *mlist_find(mlist_t * l, struct mlist_elem *start, void *data);
extern struct mlist_elem *mlist_replace(mlist_t *l, struct mlist_elem *start, void *data);

/**
 * @}
 */




#define mlist_foreach(i,l)\
	for (i=l->data; i; i=i->next)




/**
 * @}
 */

#endif
