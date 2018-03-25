#include <stdio.h>
#include "format.h"


/**
 * Read hex bytes from a string to an uint8_t array
 * @param dst destination array
 * @param s string to read
 * @param len length of string
 * @return 0 if all was ok \n 1 if an error has occured.
 */

int cw_format_scan_hex_bytes(uint8_t *dst,const char *s, int len)
{
	int rc ;
	int err=0;
	int val;
	unsigned int c;
	int i;

	if ( len & 1){
		rc = sscanf(s,"%01X",&c);
		if (rc!=1){
			c=0;
			err=1;
		}
		*dst++=c;
		s++;
		len--;
	}


	for (i=0; i<len; i++){
		rc = sscanf(s+i,"%01X",&c);
		if (rc!=1){
			c=0;
			err=1;
		}


		if (!(i&1)) {
			val = c<<4;	

		}
		else{
			val |= c;
			*dst++=val;
		}

	}
	
	return err;
}
