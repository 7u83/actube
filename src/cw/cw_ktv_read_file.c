
#include "ktv.h"

int cw_ktv_read_file(FILE * file, mavl_t ktv, mavl_t types)
{
	char key[256];
	char xtype[256];
	char val[2048];
	
	
	int rc;
	struct cw_Type typesearch, *cwtype;

	
	do {
		cw_KTV_t nktv, *nres;
		
		rc = cw_ktv_read_line(file,key,xtype,val);
		if (rc == 0){
			printf("Got a line %s:%s:%s\n",key,xtype,val);
			
		}
		
		typesearch.name=xtype;
	/*	printf("Searching for type: '%s'\n",xtype);*/
		cwtype = mavl_get_ptr(types,&typesearch);
		if (cwtype != NULL){
		//	printf("Yes! I know the type: %s\n",cwtype->name);
		}
		else {
			printf("Unknown type: %s\n",typesearch.name);
			continue;
		}
		
		
		
		cw_ktv_add_from_str(ktv,key,cwtype,val);
	
/*		nres = cwtype->from_str(&nktv,val);

		if (nres !=NULL ){
		//	printf("Yea, created from string\n");
			
			nktv.key=cw_strdup(key);
			mavl_add(ktv, &nktv, NULL);
		}
*/	
	}while(rc==0);
	
	return 0;
}
