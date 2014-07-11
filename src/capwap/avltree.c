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

#include <stdlib.h>
#include <stdio.h>


#include "avltree.h"


struct avltree * avltree_create(int (*cmp)(const void*,const void*),void(*del)(void*))
{
	struct avltree * t = malloc(sizeof(struct avltree));
	if (!t)
		return 0;
	t->root=0;
	t->count=0;
	t->cmp=cmp;
	t->del=del;
	return t;
}

struct avlnode * avlnode_create(void * data)
{
	struct avlnode * n = malloc(sizeof(struct avlnode));
	if(!n)
		return 0;
	
	n->left=n->right=0;
	n->bal=0;
	n->data = data;
	return n;
}


void avlnode_destroy(struct avltree *t,struct avlnode *n)
{
	if(t->del){
		t->del(n->data);
	}
	free(n);
	t->count--;
}


static int  avltree_add0(struct avltree *t, struct avlnode ** parent, void ** data)
{
//	struct avlnode * rn;
	struct avlnode *tmp;

	struct avlnode * n = *parent;
	int rc=t->cmp(*data,n->data);

	int bal; 
	if (rc==0){
		*data=n->data;
		return 2;
	}

	if (rc<0){
		if (n->left)
		{
			bal = avltree_add0(t,&n->left,data);
			if (bal>1)
				return bal;

			n->bal -=bal;
			if (n->bal==0)
				return 0;
			
			if (n->bal==-2){
				if (n->left->bal==-1){
					n->bal=0;
					n->left->bal=0;
					*parent=n->left;
					tmp=n->left->right;
					n->left->right=n;
					n->left=tmp;
					return 0;
				}
				if (n->left->bal==1){
					*parent=n->left->right;
					if ((*parent)->bal==1) {
						n->bal=0;
						n->left->bal=-1;
					}
					else if ((*parent)->bal==-1){
						n->bal=1;
						n->left->bal=0;
					}
					else{
						n->bal=0;
						n->left->bal=0;
					}

					(*parent)->bal=0;
					n->left->right=(*parent)->left;
					(*parent)->left=n->left;
					tmp = (*parent)->right;
					(*parent)->right=n;
					n->left=tmp;
					return 0;

				}	

				//printf("!!!!left bal = %i\n",n->left->bal);
				//exit(0);

			}
			return bal;

		}

		/* n->left is 0 */
		n->left=avlnode_create(*data);
		if (!n->left)
			return 3;

		t->count++;

		if(n->right==0){
			n->bal=-1;
			return 1;
		}

		n->bal=0;
		return 0;

	}
	else{
		if (n->right){
			bal = avltree_add0(t,&n->right,data);
			if(bal>1)
				return bal;

			n->bal+=bal;
			if (n->bal==0)
				return 0;

			if (n->bal==2){
				if (n->right->bal==1){
					n->bal=0;
					n->right->bal=0;
					*parent=n->right;
					tmp=n->right->left;
					n->right->left=n;
					n->right=tmp;
					return 0;
				}
				else if(n->right->bal==-1){
					*parent=n->right->left;

					if ((*parent)->bal==-1) {
						n->bal=0;
						n->right->bal=1;
					}
					else if ((*parent)->bal==1){
						n->bal=-1;
						n->right->bal=0;
					}
					else{
						n->bal=0;
						n->right->bal=0;
					}





					(*parent)->bal=0;
					n->right->left=(*parent)->right;
					(*parent)->right=n->right;
					tmp = (*parent)->left;
					(*parent)->left=n;
					n->right=tmp;
					return 0;
				}
				//printf("!!!!iright bal = %i\n",n->left->bal);
				//exit(0);


			}
			return bal;

		}

		/* n->right is 0 */

		n->right=avlnode_create(*data);
		if (!n->right)
			return 3;

		t->count++;
		if(n->left==0){
			n->bal=1;
			return 1;
		}
		n->bal=0;
		return 0;
	}
}



void * avltree_add(struct avltree *t, void * data)
{
	if (t->root==0){
		t->root = avlnode_create(data);
		if (t->root)
			t->count++;
		return t->root->data;
	}
	void * d = data;
	int rc = avltree_add0(t,&t->root,&d);

	if (rc>3)
		return 0;

	return d;
}

static void rot_l(struct avlnode *n, struct avlnode **parent)
{
	struct avlnode *tmp;
	*parent=n->right;
	tmp=n->right->left;
	n->right->left=n;
	n->right=tmp;
}


static void rot_r(struct avlnode *n, struct avlnode **parent)
{
	struct avlnode *tmp;
	*parent=n->left;
	tmp=n->left->right;
	n->left->right=n;
	n->left=tmp;
}					




/*
 * Delete the node withe the highest value
 * returns the rebalancing factor
 */

/*
static int avltree_delete_hi(struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;

	if(n->right!=0){
		int bal = avltree_delete_hi(&n->right,data);
		n->bal-=bal;
		if (n->bal==-2){

			//		if (rotate_r(n,parent))
			//	return 0;
		}

		return bal;
	}

	*parent=n->left;
	*data = n->data;
	if (n->left){
		free(n);
		return 0;
	}
	free(n);
	return 1;
}
*/

struct avltree * trrr;

static void rot_rl(struct avlnode *n, struct avlnode **parent)
{
	struct avlnode * tmp;
	*parent=n->right->left;
	n->right->left=(*parent)->right;
	(*parent)->right=n->right;
	tmp = (*parent)->left;
	(*parent)->left=n;
	n->right=tmp;
}

static void rot_lr(struct avlnode *n, struct avlnode **parent)
{
	struct avlnode * tmp;
	*parent=n->left->right;
	n->left->right=(*parent)->left;
	(*parent)->left=n->left;
	tmp = (*parent)->right;
	(*parent)->right=n;
	n->left=tmp;
}


static int adj_bal_l(struct avlnode *n, struct avlnode **parent)
{
	if (n->right->bal==1){
		n->bal=0;
		n->right->bal=0;
		rot_l(n,parent);
		return 1;
	}
	else if(n->right->bal==0){
		n->bal=1;
		n->right->bal=-1;
		rot_l(n,parent);
		return 0;
	}else if(n->right->bal==-1){
//		int rb;
		n->bal=0;
		n->right->bal=0;
//		rb = n->right->left->bal;
		n->right->left->bal=0;
		rot_rl(n,parent);
		return 1;
	}

//	printf("adj bal l not handled \n");
//	exit(0);

	return -11; /* that should never happen */
}

int adj_bal_r(struct avlnode *n, struct avlnode **parent)
{

	if (n->left->bal==-1){
		n->bal=0;
		n->left->bal=0;
		rot_r(n,parent);
		return 1;
	}
	else if(n->left->bal==0){
		n->bal=-1;
		n->left->bal=1;
		rot_r(n,parent);
		return 0;
	}
	else if(n->left->bal==1){
//		int rb;
		n->bal=0;
		n->left->bal=0;
//		rb = n->left->right->bal;
		n->left->right->bal=0;
		rot_lr(n,parent);
		return 1;
	}

//	printf("adj bal li left  not handled \n");
//	exit(0);
	return -11; /* that should never happen */
}




static int avltree_del_lo(struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;

	if(n->left!=0){
		int bal = avltree_del_lo(&n->left,data);
		n->bal+=bal;
		if (n->bal==1){
			return 0;
		}			
		if (n->bal!=2)
			return bal;
		adj_bal_l(n,parent);
		return 0;
	}

	/* found the lowest element */

	*parent=n->right;
	*data = n->data;
	free (n);
	return 1;


	if (n->right){
		free(n);
		return 1;
	}
	free(n);
	return 1;

}




int avltree_del0(struct avltree *t, struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;
	int rc;
	int bal;

	rc =t->cmp(*data,n->data);

	if (rc==0){
		if (n->right == 0 && n->left ==0){
			*parent=0;
			avlnode_destroy(t,n);
			return 1;
		}

		if (n->right && n->left==0){
			*parent=n->right;
			avlnode_destroy(t,n);
			return 1;

		}

		if (n->left && n->right==0){
			avlnode_destroy(t,n);
			*parent=n->left;
			return 1;

		}

		/* node has two childs */

		if (t->del){
			t->del(n->data);
		}
		t->count--;
		bal = avltree_del_lo(&n->right,&n->data);
		n->bal-= bal;
		if (n->bal==-1)
			return 0;

		if (n->bal != -2)
			return bal;

		return adj_bal_r(n,parent);

	}

	if (rc<0){
		if (n->left)
		{
			bal = avltree_del0(t,&n->left,data);
			if (bal==2)
				return 2;

			n->bal+=bal;
			if (n->bal==1)
				return 0;

			if (n->bal!=2)
				return bal;

			return adj_bal_l(n,parent);
		}
		return 2; /* not found */
	}	
	else  { /* rc must be > 0 */
		if (n->right){
			bal = avltree_del0(t,&n->right,data);
			if (bal==2)
				return 2;

			n->bal-=bal;
			if (n->bal==-1)
				return 0;

			if (n->bal != -2)
				return bal;

			return adj_bal_r(n,parent);


		}
		return 2; /* not found */

	}
	
}

void * avltree_del(struct avltree *t, void *data)
{
	void *d = data;
	int rc = avltree_del0(t,&t->root,&d);
	if (rc==2)
		return 0;
	return data;
}

/*
static int cmp(const void *k1,const void *k2)
{
	int x1 = *((int*)k1);
	int x2 = *((int*)k2);
	return x1-x2;
}
*/

//int data[]={10,37,60,10,5,35,36,26,3,11,18};
//int data[] = {100,50,75};
//int data[]={100,200,150,300,400};
//int data[]={100,200,150,170,470};
//int data[]={10,20,15,17,47,50,60,70,80};
//int data[]={9,8,7,6,5,4,3,2,1,0};

//int data[]={10,37,60,10,5,35,19,26,3,11,18};
//int data[]={0,11,14,33,37,20};
//int data[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//int data[]={1,4,3,7,5,6,7,8,9,10};
//int data[]={11,4,5,6,3,4,3,2,1,0};
//int data[]={20,16,10,11,5,4,3,2,1,0};

/*
struct avlnode * ar[10000];
int tpw = 80;

static void print_tree0(struct avlnode * n,int d,int l,int r)
{
	int pos = l+(r-l)/2;
	ar[d*tpw+pos]=n;
	if (n->right)
		print_tree0(n->right,d+1,l+(r-l)/2,r);
	if(n->left)
		print_tree0(n->left,d+1,l,r-(r-l)/2);
}

void avltree_print(struct avlnode * n )
{

	int y;
	int i=0;
	for(i=0; i<10000; i++)
		ar[i]=0;

	if (n==0){

		printf("Empty Tree\n");
		return ;
	 }

	print_tree0(n,0,0,tpw);
	for (i=0; i<10; i++) {
		for (y=0; y<tpw; y++){
			struct avlnode *r=ar[tpw*i+y];
			if(r){
				printf("%i(%i)",*(int*)(r->data),r->bal);
			}
			else{
				printf(".");
			}
		}	
		printf("\n");
	}


}



void walk(struct avlnode *n)
{
	if (n == 0)
		return;
	walk(n->left);
	int x = *((int*)(n->data));
	printf("VAL: %i\n",x);
	walk(n->right);
//	x = *((int*)(n->data));
//	printf("VALR: %i\n",x);
	
}

*/

void avltree_destroy(struct avltree *t)
{
	avltree_del_all(t);
	free (t);
}


//#include <time.h>









/*
//int data[]={5,1,9,7,0,10,8,0,4,3};
//int data[]={50,10,90,70,00,100,80,00,40,30,1,2,3};
int data[]={10,20,30,45,1,50,11,51,60,70,80,90,99,25,50,10,90,70,00,100,80,00,40,30,1,2,3};


int main()
{
	struct avltree *t = avltree_create(cmp,0);
	trrr=t;
	printf("T: %p\n",t);

	srand(time(NULL));
	int i=0;
	for (i=0; i<6; i++)
	{
		int r = rand()%0x3f; // % 0xiff;
//		int r = rand(); //%0x3f; // % 0xiff;

	//	r = data[7-i];
		r = data[i];
		int * dr = malloc(sizeof(int));
		*dr = r;


		printf("Insert %i\n",*dr);

		void * d = avltree_insert(t,dr);
		printf("After insert %i\n",r);
		print_tree(t->root);

		if (d!=dr){
//			printf("exists\n");
		}


	}

	printf("Here it is\n");
	print_tree(t->root);
//	walk(t->root);	
	void * da;

	int x = 110;

//	void * drc = avltree_delete(t,&x);
	avltree_delete_all(t);
//	printf("Delete 110 rc = %p\n",drc);
	print_tree(t->root);
	printf("Count: %i\n",t->count);
	return 0;
}



*/
