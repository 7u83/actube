


int avltree_foreach_lr_node(struct avltree *t, struct avlnode *n, void *data,int (*callback)(void *,void *),void *cbpriv)
{
	if (!n)
		return 1;

	int rc=t->cmp(data,n->data);
	if (rc<0){
		avltree_foreach_lr_node(t,n->left,data,callback,cbpriv);
		callback(cbpriv,n->data);
		avltree_foreach_lr(n->right,
		return 0;
	}

	if (rc>0) {
		avltree_foreach_lr_node(t,n->right,data,callback,cbpriv);
		return 0;
	}

	return callback(cbpriv,n->data);




}


int avltree_foreach_2(struct avltree *t,void *data,int (*callback)(void *,void *),void *cbpriv)
{

}
