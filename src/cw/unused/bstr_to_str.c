#include <stdio.h>
#include <string.h>

#include "bstr.h"
#include "cw_util.h"

int bstr_to_str(char *dst, bstr_t str,char * def)
{
	if (!str){
		if (!def) 
			return 0;
		return sprintf(dst,"%s",def);
	}

	int printable = cw_is_printable(bstr_data(str),bstr_len(str));
	int l=bstr_len(str);

	if (printable){
		memcpy((char*)dst,bstr_data(str),l);
		*(dst+l)=0;
		return l;
	}

	int i;
	int c=0;
	char *s = dst; 
	for (i=0; i<l; i++){
		if (!c){

			s += sprintf(s,"%02X",bstr_data(str)[i]);
			c=1;
		}
		else
			s += sprintf(s,",%02X",bstr_data(str)[i]);
	}	

	return s-dst;
}
