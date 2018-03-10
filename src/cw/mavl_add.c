#include "mavl.h"

static struct mavlnode *mavlnode_create ( struct mavl *t, const void *data )
{
	struct mavlnode *n = malloc ( sizeof ( struct mavlnode ) + t->data_size );
	
	if ( !n )
		return NULL;
		
	n->left = n->right = NULL;
	n->bal = 0;
	
	/*n->data = *data;*/
	memcpy ( mavlnode_dataptr ( n ), data, t->data_size );
	
	return n;
}


static int mavl_add0 ( struct mavl *t, struct mavlnode **parent, const void ** data )
{
	struct mavlnode *tmp;
	struct mavlnode *n = *parent;
	int rc = t->cmp ( *data, mavlnode_dataptr ( n ) );
	
	int bal;
	
	if ( rc == 0 ) {
		/*		*data = &n->data;*/
		
		/*		memcpy(data,mavlnode_dataptr(n),t->data_size);*/
		*data = mavlnode_dataptr ( n );
		return 2;
	}
	
	if ( rc < 0 ) {
		if ( n->left ) {
			bal = mavl_add0 ( t, &n->left, data );
			
			if ( bal > 1 )
				return bal;
				
			n->bal -= bal;
			
			if ( n->bal == 0 )
				return 0;
				
			if ( n->bal == -2 ) {
				if ( n->left->bal == -1 ) {
					n->bal = 0;
					n->left->bal = 0;
					*parent = n->left;
					tmp = n->left->right;
					n->left->right = n;
					n->left = tmp;
					return 0;
				}
				
				if ( n->left->bal == 1 ) {
					*parent = n->left->right;
					
					if ( ( *parent )->bal == 1 ) {
						n->bal = 0;
						n->left->bal = -1;
						
					} else if ( ( *parent )->bal == -1 ) {
						n->bal = 1;
						n->left->bal = 0;
						
					} else {
						n->bal = 0;
						n->left->bal = 0;
					}
					
					( *parent )->bal = 0;
					n->left->right = ( *parent )->left;
					( *parent )->left = n->left;
					tmp = ( *parent )->right;
					( *parent )->right = n;
					n->left = tmp;
					return 0;
					
				}
				
			}
			
			return bal;
			
		}
		
		/* n->left is 0 */
		n->left = mavlnode_create ( t, *data );
		*data = mavlnode_dataptr(n->left);
		
		if ( !n->left )
			return 3;
			
		t->count++;
		
		if ( n->right == 0 ) {
			n->bal = -1;
			return 1;
		}
		
		n->bal = 0;
		return 0;
		
	} else {
		if ( n->right ) {
			bal = mavl_add0 ( t, &n->right, data );
			
			if ( bal > 1 )
				return bal;
				
			n->bal += bal;
			
			if ( n->bal == 0 )
				return 0;
				
			if ( n->bal == 2 ) {
				if ( n->right->bal == 1 ) {
					n->bal = 0;
					n->right->bal = 0;
					*parent = n->right;
					tmp = n->right->left;
					n->right->left = n;
					n->right = tmp;
					return 0;
					
				} else if ( n->right->bal == -1 ) {
					*parent = n->right->left;
					
					if ( ( *parent )->bal == -1 ) {
						n->bal = 0;
						n->right->bal = 1;
						
					} else if ( ( *parent )->bal == 1 ) {
						n->bal = -1;
						n->right->bal = 0;
						
					} else {
						n->bal = 0;
						n->right->bal = 0;
					}
					
					( *parent )->bal = 0;
					n->right->left = ( *parent )->right;
					( *parent )->right = n->right;
					tmp = ( *parent )->left;
					( *parent )->left = n;
					n->right = tmp;
					return 0;
				}
			}
			
			return bal;
		}
		
		/* n->right is 0 */
		
		n->right = mavlnode_create ( t, *data );
		*data = mavlnode_dataptr(n->right);

		
		if ( !n->right )
			return 3;
			
		t->count++;
		
		if ( n->left == 0 ) {
			n->bal = 1;
			return 1;
		}
		
		n->bal = 0;
		return 0;
	}
}


/**
 * Add an element to an AVL tree
 *
 * @param t mavl
 * @param data pointer to element
 *
 * @return A pointer to the element in the tree. If teh returned pointer
 * equals to the pointer given in the data argument, the element has already
 * been in the tree. If the function returns NULL, there was en error, e.g.
 * no memory left. Consult therefore errno.
 *
 * @example mavl_add_example.c
 */

void *mavl_add ( struct mavl *t, const void *data, int *exists )
{
	const void * d;
	int rc;
	
	if ( t->root == NULL ) {
		t->root = mavlnode_create ( t, data );
		
		if ( t->root )
			t->count++;
			
		/* return &t->root->data; */
		if (exists != NULL)
			*exists=0;
		return mavlnode_dataptr(t->root);
	}
	
	d = data;
	
	rc = mavl_add0 ( t, &t->root, &d );
	
	if ( rc >= 3 )
		return NULL;
		
	if ( exists != NULL){
		if (rc==2) 
			*exists=1;
		else
			*exists=0;
	}
	return d;
}

/*
void *mavl_add ( struct mavl *t, const void *data)
{
	return mavl_add_exists(t,data,NULL);
}
*/