#include "mavl.h"

static struct mavlnode *mavlnode_create(void *data)
{
	struct mavlnode *n = malloc(sizeof(struct mavlnode));
	if (!n)
		return NULL;

	n->left = n->right = 0;
	n->bal = 0;
	n->data = data;
	return n;
}




static int mavl_add0(struct mavl *t, struct mavlnode **parent, void **data)
{
//      struct mavlnode * rn;
	struct mavlnode *tmp;

	struct mavlnode *n = *parent;
	int rc = t->cmp(*data, n->data);

	int bal;
	if (rc == 0) {
		*data = n->data;
		return 2;
	}

	if (rc < 0) {
		if (n->left) {
			bal = mavl_add0(t, &n->left, data);
			if (bal > 1)
				return bal;

			n->bal -= bal;
			if (n->bal == 0)
				return 0;

			if (n->bal == -2) {
				if (n->left->bal == -1) {
					n->bal = 0;
					n->left->bal = 0;
					*parent = n->left;
					tmp = n->left->right;
					n->left->right = n;
					n->left = tmp;
					return 0;
				}
				if (n->left->bal == 1) {
					*parent = n->left->right;
					if ((*parent)->bal == 1) {
						n->bal = 0;
						n->left->bal = -1;
					} else if ((*parent)->bal == -1) {
						n->bal = 1;
						n->left->bal = 0;
					} else {
						n->bal = 0;
						n->left->bal = 0;
					}

					(*parent)->bal = 0;
					n->left->right = (*parent)->left;
					(*parent)->left = n->left;
					tmp = (*parent)->right;
					(*parent)->right = n;
					n->left = tmp;
					return 0;

				}
				//printf("!!!!left bal = %i\n",n->left->bal);
				//exit(0);

			}
			return bal;

		}

		/* n->left is 0 */
		n->left = mavlnode_create(*data);
		if (!n->left)
			return 3;

		t->count++;

		if (n->right == 0) {
			n->bal = -1;
			return 1;
		}

		n->bal = 0;
		return 0;

	} else {
		if (n->right) {
			bal = mavl_add0(t, &n->right, data);
			if (bal > 1)
				return bal;

			n->bal += bal;
			if (n->bal == 0)
				return 0;

			if (n->bal == 2) {
				if (n->right->bal == 1) {
					n->bal = 0;
					n->right->bal = 0;
					*parent = n->right;
					tmp = n->right->left;
					n->right->left = n;
					n->right = tmp;
					return 0;
				} else if (n->right->bal == -1) {
					*parent = n->right->left;

					if ((*parent)->bal == -1) {
						n->bal = 0;
						n->right->bal = 1;
					} else if ((*parent)->bal == 1) {
						n->bal = -1;
						n->right->bal = 0;
					} else {
						n->bal = 0;
						n->right->bal = 0;
					}





					(*parent)->bal = 0;
					n->right->left = (*parent)->right;
					(*parent)->right = n->right;
					tmp = (*parent)->left;
					(*parent)->left = n;
					n->right = tmp;
					return 0;
				}
				//printf("!!!!iright bal = %i\n",n->left->bal);
				//exit(0);


			}
			return bal;

		}

		/* n->right is 0 */

		n->right = mavlnode_create(*data);
		if (!n->right)
			return 3;

		t->count++;
		if (n->left == 0) {
			n->bal = 1;
			return 1;
		}
		n->bal = 0;
		return 0;
	}
}


/**
 * Add an element to an AVL tree
 * @t mavl
 * @data pointer to element
 * @return added alement or NULL if error.
 */
void *mavl_add(struct mavl *t, void *data)
{
	if (t->root == 0) {
		t->root = mavlnode_create(data);
		if (t->root)
			t->count++;
		return t->root->data;
	}
	void *d = data;
	int rc = mavl_add0(t, &t->root, &d);

	if (rc > 3)
		return NULL;

	return d;
}


