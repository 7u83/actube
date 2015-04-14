
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstr.h"


/**
 * Create a bstr1616_t string from a string read from config file.
 * 
 * @param s String from config file.
 * @return The create bstr16_t string.
 *
 * The string from config file is an ASCII-text which is interpreted
 * as hexadecimal string if it starts with ".x"
 *
 * @see bstr16_t
 */
uint8_t * bstr16cfgstr(const char * s)
{
	int l = strlen(s);

	
	if (s[0]!='.')
		return bstr16_create((uint8_t*)s,l);

	if (l<=2)
		return bstr16_create((uint8_t*)s,l);

	if (s[1]=='.')
		return bstr16_create((uint8_t*)s+1,l-1);

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


		return bstr16_create(ns,len);


	}
	return NULL;
}


