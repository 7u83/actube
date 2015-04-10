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
 *@brief Implementation of avliter_next
 */

#include "avltree.h"

/**
 * Get the next element within an AVL Tree.
 * @param i pointer to AVL Iterator
 * @return the element or NULL if there is no next elemeent.
 */ 
void * avliter_next(avliter_t *i)
{

	while ( i->stack_ptr) {
		i->stack_ptr--;
		i->cur=i->stack[i->stack_ptr];
			
		if (!i->cur)
			continue;

		if ((i->stack_ptr)&1) {
			return i->cur->data;
		}
		break;		

	}


	if (!i->cur){
		return NULL;
	}

	while(i->cur->left) {
		/* push right branch */
		i->stack[i->stack_ptr++]=i->cur->right;
		
		/* push node */
		i->stack[i->stack_ptr++]=i->cur;

		i->cur=i->cur->left;

	}

	i->stack[i->stack_ptr++]=i->cur->right;

	return i->cur->data;
}


