#include "avltree.h"


// XXX Function is buggy

int avltree_foreach_from_lr(struct avltree *t, struct avlnode *n, void *data,int (*callback)(void *,void *),void *cbpriv)
{
        if (!n)
                return 1;

        int rc=t->cmp(data,n->data);
        if (rc<0){
                if(!avltree_foreach_from_lr(t,n->left,data,callback,cbpriv))
			return 0;
                if (!callback(cbpriv,n->data))
			return 0;
                return avltree_foreach_lr(n->right,callback,cbpriv);
        }

        if (rc>0) {
                avltree_foreach_from_lr(t,n->right,data,callback,cbpriv);
                return 0;
        }

        if (!callback(cbpriv,n->data))
		return 0;

	return avltree_foreach_lr(n->right,callback,cbpriv);
}


