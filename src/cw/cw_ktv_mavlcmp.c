
#include <ctype.h>

#include "ktv.h"
/**
 * @brief Default function to compare two values of type #cw_KTV_t.
 *
 * @param v1
 * @param v2
 * @return
 */
int cw_ktv_mavlcmp (const void *v1, const void *v2)
{
	char *d1, *d2, *k1, *k2;
	int l1, l2, rc, i1, i2;
	
	k1 = ( (cw_KTV_t *) v1)->key;
	k2 = ( (cw_KTV_t *) v2)->key;
	
	while (1) {
		/* Find dots in both keys */
		d1 = strchr (k1, '.');
		d2 = strchr (k2, '.');
		
		/* if there are no dots, compare keys as normal */
		if (d1 == NULL || d2 == NULL)
			return strcmp (k1, k2);
			
			
		/* calculate the length of the key till dots */
		l1 = d1 - k1; /*((cw_KTV_t *) v1)->key;*/
		l2 = d2 - k2; /*((cw_KTV_t *) v2)->key;*/
		
		/* if length differs do a normal compare */
		if (l1 != l2) {
			return strcmp (k1, k2); /*((cw_KTV_t *) v1)->key, ((cw_KTV_t *) v2)->key);*/
		}
		
		
		rc = strncmp (k1, k2, l1); /*((cw_KTV_t *) v1)->key,((cw_KTV_t *) v2)->key,l1);*/
		
		if (rc != 0) {
			return rc;
		}
		
		d1++;
		d2++;
		i1 = atoi (d1);
		i2 = atoi (d2);
		
		rc = i1 - i2;
		
		if (rc != 0)
			return rc;
			
		while (isdigit (*d1))
			d1++;
			
		while (isdigit (*d2))
			d2++;
		
		k1=d1;
		k2=d2;
		/*return strcmp(d1,d2);*/
		/*return cw_ktv_mavlcmp(d1,d2);*/
	}
}
