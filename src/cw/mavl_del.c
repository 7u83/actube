#include "mavl.h"

void mavlnode_destroy(struct mavl *t, struct mavlnode *n)
{
	if (t->del) {
		t->del(n->data);
	}
	free(n);
	t->count--;
}




static void rot_rl(struct mavlnode *n, struct mavlnode **parent)
{
	struct mavlnode *tmp;
	*parent = n->right->left;
	n->right->left = (*parent)->right;
	(*parent)->right = n->right;
	tmp = (*parent)->left;
	(*parent)->left = n;
	n->right = tmp;
}

static void rot_lr(struct mavlnode *n, struct mavlnode **parent)
{
	struct mavlnode *tmp;
	*parent = n->left->right;
	n->left->right = (*parent)->left;
	(*parent)->left = n->left;
	tmp = (*parent)->right;
	(*parent)->right = n;
	n->left = tmp;
}



static void rot_l(struct mavlnode *n, struct mavlnode **parent)
{
	struct mavlnode *tmp;
	*parent = n->right;
	tmp = n->right->left;
	n->right->left = n;
	n->right = tmp;
}


static void rot_r(struct mavlnode *n, struct mavlnode **parent)
{
	struct mavlnode *tmp;
	*parent = n->left;
	tmp = n->left->right;
	n->left->right = n;
	n->left = tmp;
}



static int adj_bal_l(struct mavlnode *n, struct mavlnode **parent)
{
	if (n->right->bal == 1) {
		n->bal = 0;
		n->right->bal = 0;
		rot_l(n, parent);
		return 1;
	} else if (n->right->bal == 0) {
		n->bal = 1;
		n->right->bal = -1;
		rot_l(n, parent);
		return 0;
	} else if (n->right->bal == -1) {
//              int rb;
		n->bal = 0;
		n->right->bal = 0;
//              rb = n->right->left->bal;
		n->right->left->bal = 0;
		rot_rl(n, parent);
		return 1;
	}
//      printf("adj bal l not handled \n");
//      exit(0);

	return -11;		/* that should never happen */
}

static int adj_bal_r(struct mavlnode *n, struct mavlnode **parent)
{

	if (n->left->bal == -1) {
		n->bal = 0;
		n->left->bal = 0;
		rot_r(n, parent);
		return 1;
	} else if (n->left->bal == 0) {
		n->bal = -1;
		n->left->bal = 1;
		rot_r(n, parent);
		return 0;
	} else if (n->left->bal == 1) {
//              int rb;
		n->bal = 0;
		n->left->bal = 0;
//              rb = n->left->right->bal;
		n->left->right->bal = 0;
		rot_lr(n, parent);
		return 1;
	}
//      printf("adj bal li left  not handled \n");
//      exit(0);
	return -11;		/* that should never happen */
}






static int mavl_del_lo(struct mavlnode **parent, void **data)
{
	struct mavlnode *n = *parent;

	if (n->left != 0) {
		int bal = mavl_del_lo(&n->left, data);
		n->bal += bal;
		if (n->bal == 1) {
			return 0;
		}
		if (n->bal != 2)
			return bal;
		adj_bal_l(n, parent);
		return 0;
	}

	/* found the lowest element */

	*parent = n->right;
	*data = n->data;
	free(n);
	return 1;


	if (n->right) {
		free(n);
		return 1;
	}
	free(n);
	return 1;

}




int mavl_del0(struct mavl *t, struct mavlnode **parent, void **data)
{
	struct mavlnode *n = *parent;
	int rc;
	int bal;
	rc = t->cmp(*data, n->data);

	if (rc == 0) {
		if (n->right == 0 && n->left == 0) {
			*parent = 0;
			mavlnode_destroy(t, n);
			return 1;
		}

		if (n->right && n->left == 0) {
			*parent = n->right;
			mavlnode_destroy(t, n);
			return 1;

		}

		if (n->left && n->right == 0) {
			mavlnode_destroy(t, n);
			*parent = n->left;
			return 1;

		}

		/* node has two childs */

		if (t->del) {
			t->del(n->data);
		}
		t->count--;
		bal = mavl_del_lo(&n->right, &n->data);
		n->bal -= bal;
		if (n->bal == -1)
			return 0;

		if (n->bal != -2)
			return bal;

		return adj_bal_r(n, parent);

	}

	if (rc < 0) {
		if (n->left) {
			bal = mavl_del0(t, &n->left, data);
			if (bal == 2)
				return 2;

			n->bal += bal;
			if (n->bal == 1)
				return 0;

			if (n->bal != 2)
				return bal;

			return adj_bal_l(n, parent);
		}
		return 2;	/* not found */
	} else {		/* rc must be > 0 */
		if (n->right) {
			bal = mavl_del0(t, &n->right, data);
			if (bal == 2)
				return 2;

			n->bal -= bal;
			if (n->bal == -1)
				return 0;

			if (n->bal != -2)
				return bal;

			return adj_bal_r(n, parent);


		}
		return 2;	/* not found */

	}

}

void *mavl_del(struct mavl *t, void *data)
{
	if (!t->root)
		return NULL;

	void *d = data;
	int rc = mavl_del0(t, &t->root, &d);
	if (rc == 2)
		return NULL;
	return data;
}


