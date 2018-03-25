
#include "lwapp.h"
#include "lw.h"

int lw_put_image_data(uint8_t *dst,FILE *infile)
{
	uint16_t checksum;
	int bytes;
	if ( feof(infile)){
		lw_set_byte(dst+0,1);
		return 1;
	}

	bytes = fread(dst+3,1,LWAPP_BLOCKSIZE_IMAGE_DATA,infile);

	if ( ferror(infile)) {
		lw_set_byte(dst+0,5);
		return 1;
	}

	lw_set_byte(dst,3); 

	checksum = lw_checksum(dst+3,bytes);
	lw_set_word(dst+1,checksum);
		
	return bytes+3;
}


