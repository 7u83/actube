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

/**
 * @file
 * @brief Yet another AVL tree implementation
 */

#include <stdlib.h>
#include <stdio.h>


#include "mavl.h"





/*
 * Delete the node with the highest value
 * returns the rebalancing factor
 */

/*
static int mavl_delete_hi(struct mavlnode **parent, void **data)
{
	struct mavlnode * n = *parent;

	if(n->right!=0){
		int bal = mavl_delete_hi(&n->right,data);
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

//struct mavl * trrr;


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
struct mavlnode * ar[10000];
int tpw = 80;

static void print_tree0(struct mavlnode * n,int d,int l,int r)
{
	int pos = l+(r-l)/2;
	ar[d*tpw+pos]=n;
	if (n->right)
		print_tree0(n->right,d+1,l+(r-l)/2,r);
	if(n->left)
		print_tree0(n->left,d+1,l,r-(r-l)/2);
}

void mavl_print(struct mavlnode * n )
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
			struct mavlnode *r=ar[tpw*i+y];
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



void walk(struct mavlnode *n)
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

/*
void mavl_destroy(struct mavl *t)
{
	mavl_del_all(t);
	free (t);
}
*/


//#include <time.h>









/*
//int data[]={5,1,9,7,0,10,8,0,4,3};
//int data[]={50,10,90,70,00,100,80,00,40,30,1,2,3};
int data[]={10,20,30,45,1,50,11,51,60,70,80,90,99,25,50,10,90,70,00,100,80,00,40,30,1,2,3};


int main()
{
	struct mavl *t = mavl_create(cmp,0);
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

		void * d = mavl_insert(t,dr);
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

//	void * drc = mavl_delete(t,&x);
	mavl_delete_all(t);
//	printf("Delete 110 rc = %p\n",drc);
	print_tree(t->root);
	printf("Count: %i\n",t->count);
	return 0;
}



*/
