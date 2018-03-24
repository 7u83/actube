
#include "mavl.h"

struct mavlnode * mavl_get_node_cmp(struct mavl *t ,void *data,
	int ( *cmp ) ( const void *, const void * ))
{
	struct mavlnode *n = t->root;
	while(n){
		int rc=cmp(data,mavlnode_dataptr(n));
		if (rc==0)
			return n;
		if (rc<0)
			n=n->left;
		else
			n=n->right;
	}
	return NULL;
}
