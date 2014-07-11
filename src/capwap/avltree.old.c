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

/*
struct avlnode{
	void * data;
	struct avlnode * left;
	struct avlnode * right;
	int bal;
};

struct avltree{
	struct avlnode * root;
	int (*cmp)(const void*,const void*);
	void(*del)(void*);
	int count;
};

*/


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


int  avltree_insert0(struct avltree *t, struct avlnode ** parent, void ** data)
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
			bal = avltree_insert0(t,&n->left,data);
			if (bal>1)
				return bal;
			printf("Left ret bal %i\n",bal);

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
//					printf("Left rot a\n");
//					print_tree(t->root);
					return 0;
				}
				if (n->left->bal==1){
					printf("BUMBUMBUM\n");
					print_tree(t->root);
//					n->bal=0;
//					n->left->bal=0;
					*parent=n->left->right;
					if ((*parent)->bal==1) {
						n->bal=0;
						n->left->bal=-1;
					}
					else{
						n->bal=1;
						n->left->bal=0;
					}

					(*parent)->bal=0;

					n->left->right=(*parent)->left;
					(*parent)->left=n->left;
					tmp = (*parent)->right;
					(*parent)->right=n;
					n->left=tmp;
					printf("Left rot b\n");
//					print_tree(t->root);
					return 0;

				}	

				print_tree(t->root);
				printf("!!!!left bal = %i\n",n->left->bal);
				exit(0);

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
			bal = avltree_insert0(t,&n->right,data);
			if(bal>1)
				return bal;

			printf("Right ret bal %i\n",bal);
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
//					printf("Right rot a\n");
//					print_tree(t->root);
					return 0;
				}
				else {
					printf("Right rot b before\n");
//					print_tree(t->root);
					n->bal=0;
					n->right->bal=0;
					*parent=n->right->left;
					(*parent)->bal=0;
					n->right->left=(*parent)->right;
					(*parent)->right=n->right;
					tmp = (*parent)->left;
					(*parent)->left=n;
					n->right=tmp;
//					printf("Right rot b\n");
//					print_tree(t->root);
					return 0;
				}

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



void * avltree_insert(struct avltree *t, void * data)
{

	if (t->root==0){
		t->root = avlnode_create(data);
		return t->root->data;
	}
	void * d = data;
	int rc = avltree_insert0(t,&t->root,&d);

	if (rc>3)
		return 0;

	return d;
//	printf("RC %i\n",rc);
}



static int rotate_r(struct avlnode *n, struct avlnode **parent)
{
	if (n->bal!=-2)
		return 0;

	struct avlnode * tmp;

	if (n->bal==-1){
		n->bal=0;
		n->left->bal=0;
		*parent=n->left;
		tmp=n->left->right;
		n->left->right=n;
		n->left=tmp;
		return 1;
	}

	else /* if (n->left->bal==1)*/ {
		n->bal=0;
		n->left->bal=0;
		*parent=n->left->right;
		n->left->right=(*parent)->left;
		(*parent)->left=n->left;
		tmp = (*parent)->right;
		(*parent)->right=n;
		n->left=tmp;
		return 1;
	}

}


static int rotate_l(struct avlnode *n, struct avlnode **parent)
{
	if (n->bal!=2)
		return 0;
	
	struct avlnode * tmp;
	if (n->right->bal==1){
		n->bal=0;
		n->right->bal=0;
		*parent=n->right;
		tmp=n->right->left;
		n->right->left=n;
		n->right=tmp;
		return 1;
	}
	else {	/* n->bal musst be -1 */
		n->bal=0;
		n->right->bal=0;
		*parent=n->right->left;
		n->right->left=(*parent)->right;
		(*parent)->right=n->right;
		tmp = (*parent)->left;
		(*parent)->left=n;
		n->right=tmp;
		return 1;
	}

}



/*
 * Delete the node withe the highest value
 * returns the rebalancing factor
 */
static int avltree_delete_hi(struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;

	if(n->right!=0){
		int bal = avltree_delete_hi(&n->right,data);
		n->bal-=bal;
		if (rotate_r(n,parent))
				return 0;

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


static int avltree_delete_lo(struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;

	if(n->left!=0){
		int bal = avltree_delete_lo(&n->left,data);
		n->bal+=bal;
		if (rotate_l(n,parent))
				return 0;

		return bal;
	}

	*parent=n->right;
	*data = n->data;
	if (n->right){
		free(n);
		return 0;
	}
	free(n);
	return 1;

}




int avltree_delete0(struct avltree *t, struct avlnode **parent, void **data)
{
	struct avlnode * n = *parent;
	int rc;

	rc =t->cmp(*data,n->data);

	if (rc==0){
		if (n->right == 0 && n->left ==0){
			printf("My case a!\n");
			*parent=0;
			return 1;
		}


		if (n->right && n->left==0){
			printf("My case b!\n");
			*parent=n->right;
			return 1;

		}

		if (n->left && n->right==0){
			printf("My case c!\n");

			*parent=n->left;
			return 1;

		}

		

	}

	int bal;
	if (rc<0){
		if (n->left)
		{
			bal = avltree_delete0(t,&n->left,data);
			printf("Ballla = %i\n",bal);
			n->bal+=bal;
			if (!rotate_l(n,parent))
				return bal;
			return bal;


		}
	}	
	else {
		if (n->right){
			printf("Bolla\n");
			bal = avltree_delete0(t,&n->right,data);
			n->bal-=bal;
			if (!rotate_r(n,parent))
				return bal;
			return bal;

		}

	}


	return 0;
	
}

void * avltree_delete(struct avltree *t, void *data)
{
	void *d = data;

	printf("deletelel %p\n",d);

	avltree_delete0(t,&t->root,&d);
	return 0;
}

static int cmp(const void *k1,const void *k2)
{
	int x1 = *((int*)k1);
	int x2 = *((int*)k2);
	return x1-x2;
}

int data[]={10,37,60,10,5,35,36,26,3,11,18};
//int data[]={10,37,60,10,5,35,19,26,3,11,18};
//int data[]={0,11,14,33,37,20};
//int data[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//int data[]={1,4,3,7,5,6,7,8,9,10};
//int data[]={11,4,5,6,3,4,3,2,1,0};
//int data[]={20,16,10,11,5,4,3,2,1,0};

struct avlnode * ar[10000];
int tpw = 100;

void print_tree0(struct avlnode * n,int d,int l,int r)
{
	int pos = l+(r-l)/2;
	ar[d*tpw+pos]=n;
	if (n->right)
		print_tree0(n->right,d+1,l+(r-l)/2,r);
	if(n->left)
		print_tree0(n->left,d+1,l,r-(r-l)/2);
}

void print_tree(struct avlnode * n )
{
	int y;
	int i=0;
	for(i=0; i<10000; i++)
		ar[i]=0;

	if (n==0){

		printf("Empty Tree\n");
		return 0;
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


#include <time.h>

int main()
{
	struct avltree *t = avltree_create(cmp,0);

	printf("T: %p\n",t);

	srand(time(NULL));
	int i=0;
	for (i=0; i<7; i++)
	{
		int r = rand()%0x3f; // % 0xiff;

		r = data[7-i];
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

	print_tree(t->root);
	walk(t->root);	
	void * da;

//	avltree_delete(t,&data[4]);
	avltree_delete_hi(&t->root,&da);
//	printf("after del\n");
//	print_tree(t->root);


//	avltree_delete(t,&data[7]);
	avltree_delete_lo(&t->root,&da);
//	printf("after del\n");
//	print_tree(t->root);



//	avltree_delete(t,&data[5]);
	avltree_delete_lo(&t->root,&da);
//	printf("after del\n");
//	print_tree(t->root);
	return 0;
}




