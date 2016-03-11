
#include "cw.h"



int cw_put_image_data(uint8_t *dst,FILE *infile)
{
	int bytes = fread(dst+1,1,1024,infile);

	if (ferror(infile)){
		cw_put_byte(dst+0,5);
		return 1;
	}

	if ( feof(infile)){
		/* Last image block */
		cw_put_byte(dst,2); 
	}
	else{
		cw_put_byte(dst,1);
	}
		
	return bytes+1;
}


