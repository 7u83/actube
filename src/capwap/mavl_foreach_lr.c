#include "mavl.h"



int mavl_foreach_lr(struct mavlnode *n, int (*callback)(void *,void *),void *cbpriv)
{
	if (!n)
		return 1;
	if (!mavl_foreach_lr(n->left,callback,cbpriv))
		return 0;
	if (!callback(cbpriv,n->data))
		return 0;
	return mavl_foreach_lr(n->right,callback,cbpriv);

}


