
#include "ktv.h"


int cw_ktv_read_file(FILE * file, mavl_t ktv, mavl_t types)
{
	char key[CW_KTV_MAX_KEY_LEN];
	char type[256];
	char val[2048];
	
	
	int rc;
	struct cw_Type typesearch, *cwtype;

	
	do {

		rc = cw_ktv_read_line(file,key,type,val);
		if (rc != 0){
			continue;
		}

		typesearch.name=type;

		cwtype = mavl_get_ptr(types,&typesearch);
		if (cwtype != NULL){
		/*//	printf("Yes! I know the type: %s\n",cwtype->name);*/
		}
		else {
			printf("Unknown type: %s\n",typesearch.name);
			continue;
		}

		cw_ktv_add_from_str(ktv,key,cwtype,NULL,val);
	

	}while(rc==0);
	
	return 0;
}
