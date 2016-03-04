#include "lwapp.h"
#include "lw.h"

int lw_put_image_data(uint8_t *dst,FILE *infile)
{
	if ( feof(infile)){
		lw_put_byte(dst+0,1);
		return 1;
	}

	int bytes = fread(dst+3,1,LW_BLOCKSIZE_IMAGE_DATA,infile);

	if ( ferror(infile)) {
		lw_put_byte(dst+0,5);
		return 1;
	}

	lw_put_byte(dst,3); 

	uint16_t checksum = lw_checksum(dst+3,bytes);
	lw_put_word(dst+1,checksum);
		
	return bytes+3;
}


