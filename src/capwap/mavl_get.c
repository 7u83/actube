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
 * @file
 * @brief Implementation of mavl_get
 */ 

#include <stdio.h>

#include "mavl.h"

/**
 * Get an AVL tree element.
 * @param data Element to get
 * @return pointer to element or NULL if not found. 
 */ 
void * mavl_get(struct mavl *t ,void *data)
{
	struct mavlnode *n = t->root;
	while(n){
		int rc=t->cmp(data,n->data);
		if (rc==0)
			return n->data;
		if (rc<0)
			n=n->left;
		else
			n=n->right;
	}
	return NULL;
}


