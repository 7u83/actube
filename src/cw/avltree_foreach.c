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

#include "avltree.h"

int avltree_foreach_rl(struct avlnode *n, int (*callback)(void *,void *),void *cbpriv)
{
	if (!n)
		return 1;
	if (!avltree_foreach_rl(n->right,callback,cbpriv))
		return 0;
	if (!callback(cbpriv,n->data))
		return 0;
	return avltree_foreach_rl(n->left,callback,cbpriv);
}

void avltree_foreach(struct avltree *t, int (*callback)(void *,void *),void * cbpriv,int dir)
{
	if (dir)
		avltree_foreach_lr(t->root,callback,cbpriv);
	else
		avltree_foreach_rl(t->root,callback,cbpriv);
}

