#include "ktv.h"

void * ktvn(struct mavl *t ,const void *search)
{
	
	struct mavlnode *n,/**lastl,*/*lastb;
	lastb = NULL; /*lastl=NULL;*/
	n = t->root;
	while(n){
		int rc;
/*		const cw_KTV_t;*//* *c1,*c2;*/
		/*c1=search;
		c2=mavlnode_dataptr(n);
		*/
		
		rc = t->cmp(search,mavlnode_dataptr(n));
		
		/*printf("Compare: %s %s = %d\n",c1->key,c2->key, rc);*/
		
		if (rc==0){
			return NULL;
			
		}
		
		if (rc<0){
			/*lastl = n;*/
			if (n->left==NULL){
				return mavlnode_dataptr(lastb);
				
			}
			n=n->left;
		}
		else{
			lastb=n;
			if(n->right==NULL){
				return mavlnode_dataptr(lastb);
				
			}
			n=n->right;
		}
	}
	return NULL;
	
}




int cw_ktv_idx_get(mavl_t ktv, const char *key)
{
	char ikey[CW_KTV_MAX_KEY_LEN];
	cw_KTV_t search, * result;
	char *d;
	
	sprintf(ikey,"%s.%d",key,65536);
		
	search.key=ikey;
	/*//result = ktvn(ktv,&search);*/
	
	result = mavl_get_last(ktv,&search);
	
	if (result == NULL){
		return -1;
	}
	
	d = strchr(result->key,'.');
	if (d==NULL){
		return -1;
	}
	
	if (strncmp(result->key,ikey,d-result->key)!=0)
		return -1;
	
	return atoi(d+1);
}
