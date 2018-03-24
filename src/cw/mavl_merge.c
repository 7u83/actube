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

#include "mavl.h"


/** 
 * @file
 * @brief Implementation if mavl_merge.
 * @{
 */

static void mavlnode_move(mavl_t m,mavl_t t, struct mavlnode *n)
{
	struct mavlnode * mn = mavlnode_get(m,mavlnode_dataptr(n));
	if (mn) {
		if (m->del) {
			m->del(mavlnode_dataptr(mn));
		}
		/*mn->data=n->data;*/
		memcpy(mavlnode_dataptr(mn),mavlnode_dataptr(n),t->data_size);
	}
	else{
		/*mavl_add(m,&n->data);*/
		mavl_add(m,mavlnode_dataptr(n),NULL);

	}
	free(n);
	t->count--;
}



static void mavl_merge0(mavl_t m, mavl_t t ,struct mavlnode * n)
{
	if (!n)
		return;
	mavl_merge0(m,t,n->left);
	mavl_merge0(m,t,n->right);
	mavlnode_move(m,t,n);
}


/**
 * Merge two MAVL Objects
 *
 * Move all elements in MAVL object t to MAVL object m. Delete all
 * elements from MAVL object t. If an element from t already exists
 * in m, it will be replaced.
 *
 * @param m target object 
 * @param t source object
 * 
 * 
 */ 
void mavl_merge(mavl_t m, mavl_t t)
{
	mavl_merge0(m,t,t->root);		
	t->root=NULL;
}


