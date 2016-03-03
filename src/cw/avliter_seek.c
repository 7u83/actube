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
 *@brief Implementation of avliter_seek
 */


#include "avltree.h"

/**
 * Set an AVL Iterator to a specific position.
 * @param i AVL Iterator
 * @param d element to search for
 * @return element found or NULL if not found
 */
void * avliter_seek(avliter_t *i,void *d)
{
	if (!i->root)
		return NULL;

	i->cur=i->root;
	i->stack_ptr=0;
		
	int rc;
	while(i->cur) {

		rc = i->cmp(d,i->cur->data);
		if (rc==0){
			i->stack[i->stack_ptr++]=i->cur->right;
			i->stack[i->stack_ptr++]=i->cur;
			return avliter_next(i);
		}

		if (rc<0) {
			i->stack[i->stack_ptr++]=i->cur->right;
			i->stack[i->stack_ptr++]=i->cur;

			i->cur=i->cur->left;

		}
		if (rc>0) {

			i->cur=i->cur->right;
		}
	}
	return NULL;
}


