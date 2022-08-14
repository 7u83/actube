#include "val.h"
#include "cfg.h"

void * ktvn(struct mavl *t ,const void *search)
{
	
	struct mavlnode *n,/**lastl,*/*lastb;
	lastb = NULL; /*lastl=NULL;*/
	n = t->root;
	while(n){
		int rc;
/*		const cw_Val_t;*//* *c1,*c2;*/
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
			if (n->s[0]==NULL){
				return mavlnode_dataptr(lastb);
				
			}
			n=n->s[0];
		}
		else{
			lastb=n;
			if(n->s[1]==NULL){
				return mavlnode_dataptr(lastb);
				
			}
			n=n->s[1];
		}
	}
	return NULL;
	
}

int cw_ktv_idx_get(mavl_t ktv, const char *key)
{
	char ikey[CW_CFG_MAX_KEY_LEN];
	cw_Val_t search, * result;
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



int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n)
{
	char ikey[CW_CFG_MAX_KEY_LEN];
	cw_Val_t search, * result;
	char *d;
	int i;
	
	sprintf(ikey,"%s.%d",key,n);
		
	search.key=ikey;
	/*//result = ktvn(ktv,&search);*/
	
	result = mavl_get_first(ktv,&search);
	
	if (result == NULL){
		return -1;
	}
	
	/*d = strchr(result->key,'.');*/
	d=NULL;
/*	for (i = strlen(result->key); i>=0; i--){
		if (result->key[i]=='/')
			break;
	}
*/	for (i = strlen(ikey); i>=0; i--){

		if (ikey[i]=='.'){
			d = result->key+i;
			break;
		}
	}
	
	
	if (d==NULL){
		return -1;
	}
	
	if(result->key[i]!='.'){
		return -1;
	}
	
/*	if (strncmp(result->key,ikey,d-result->key)!=0)
		return -1;
*/
	if (strncmp(result->key,ikey,i)!=0)
		return -1;
	

	return atoi(result->key+i+1);
}
