#include "ktv.h"
/**
 * @brief Default function to compare two values of type #cw_KTV_t.
 *  
 * @param v1
 * @param v2
 * @return 
 */
int cw_ktv_mavlcmp(const void *v1, const void *v2)
{
	char *d1,*d2;
	int l1,l2,rc,i1,i2;
	
	/* Find dots in both keys */
	d1 = strchr(((cw_KTV_t *) v1)->key,'.');
	d2 = strchr(((cw_KTV_t *) v2)->key,'.');
	
	/* if there are no dots, compare keys as normal */
	if (d1==NULL || d2==NULL)
		return strcmp(((cw_KTV_t *) v1)->key, ((cw_KTV_t *) v2)->key);
	
	/* calculate the length of the key till dots */
	l1=d1-((cw_KTV_t *) v1)->key;
	l2=d2-((cw_KTV_t *) v2)->key;
	
	/* if length differs do a normal compare */
	if (l1!=l2){
		return strcmp(((cw_KTV_t *) v1)->key, ((cw_KTV_t *) v2)->key);
	}
	
	
	rc = strncmp(((cw_KTV_t *) v1)->key,((cw_KTV_t *) v2)->key,l1);
	if (rc!=0){
		return rc;
	}
	
	i1 = atoi(d1+1);
	i2 = atoi(d2+1);
	
	return i1-i2;
}
