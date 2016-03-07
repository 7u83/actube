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
 * @brief Implements mavl_del_all 
 */

#include "mavl.h"

static void mavl_del_all0(struct mavl *t ,struct mavlnode * n)
{
	if (!n)
		return;
	mavl_del_all0(t,n->left);
	mavl_del_all0(t,n->right);
	mavlnode_destroy(t,n);
}

/**
 * Delete all elemets of a mavl object
 * @parm t mavl object 
 */
void mavl_del_all(struct mavl *t)
{
	mavl_del_all0(t,t->root);		
	t->root=NULL;
}

