#include "mavl.h"



void * mavl_get_ext(struct mavl *t ,const void *search, int mode)
{
	
	struct mavlnode *n,/**lastl,*/*last;
	last = NULL; /*lastl=NULL;*/
	n = t->root;
	while(n){
		int rc;
		
		rc = t->cmp(search,mavlnode_dataptr(n));
		
		/*printf("Compare: %s %s = %d\n",c1->key,c2->key, rc);*/
		
		if (rc==0){
			return mavlnode_dataptr(n);
			
		}
		
		if (rc<0){
			if (mode == MAVL_FIND_FIRST)
				last = n;
			if (n->left==NULL){
				if (last == NULL)
					return NULL;
				return mavlnode_dataptr(last);
				
			}
			n=n->left;
		}
		else{
			if (mode == MAVL_FIND_LAST)
				last=n;
			
			if(n->right==NULL){
				if (last == NULL)
					return NULL;
				return mavlnode_dataptr(last);
				
			}
			n=n->right;
		}
	}
	return NULL;
}

