



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


