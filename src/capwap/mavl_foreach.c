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

int mavl_foreach_rl(struct mavlnode *n, int (*callback)(void *,void *),void *cbpriv)
{
	if (!n)
		return 1;
	if (!mavl_foreach_rl(n->right,callback,cbpriv))
		return 0;
	if (!callback(cbpriv,n->data))
		return 0;
	return mavl_foreach_rl(n->left,callback,cbpriv);
}

void mavl_foreach(struct mavl *t, int (*callback)(void *,void *),void * cbpriv,int dir)
{
	if (dir)
		mavl_foreach_lr(t->root,callback,cbpriv);
	else
		mavl_foreach_rl(t->root,callback,cbpriv);
}

