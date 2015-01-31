#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstr.h"



uint8_t * bstr_create_from_cfgstr(const char * s)
{
	int l = strlen(s);

	
	if (s[0]!='.')
		return bstr_create((uint8_t*)s,l+1);

	if (l<=2)
		return bstr_create((uint8_t*)s,l+1);

	if (s[1]=='.'){
		return bstr_create((uint8_t*)s+1,l);
	}

	if (s[1]=='x'){
		uint8_t * ns=0;
		int len=0;

		int ch,cl;
		const char *ss = s+2;
		int rc ;
		do {
			rc = sscanf(ss,"%01X",&ch);
			if (rc!=1)
				break;
			ss++;
			rc = sscanf(ss,"%01X",&cl);
			if (rc!=1)
				cl=0;
			ss++;
			int c=(ch<<4) | cl;
			
			len++;
			ns = realloc(ns,len);
			ns[len-1]=c;


		}while (rc==1);


		return bstr_create(ns,len);


	}

/*	if (strcmp(s,".reflect")==0){
		free(*si);
		*si=0;
		*l=0;
		return 1;
	}
*/
	return NULL;
}


