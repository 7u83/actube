/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
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

/**
 *@file
 *@Implementation of mavl_create
 *@addtogroup MavlFunctions 
 *@{
 */ 
#include "mavl.h"

/**
 * Create an AVL tree
 * @param cmp pointer compare function
 * @param del pointer to delete function which is called when an element will be deletet
 * @return pointer to an #mavl struct
 */
struct mavl *mavl_create(int (*cmp) (const void *, const void *),
			       void (*del) (void *))
{
	struct mavl *t = malloc(sizeof(struct mavl));
	if (!t)
		return NULL;
	t->root = NULL;
	t->count = 0;
	t->cmp = cmp;
	t->del = del;
	return t;
}

/**@}*/

