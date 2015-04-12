#include <stdio.h>
#include <errno.h>

#include "capwap.h"
#include "capwap_items.h"

#include "log.h"


#include "sock.h"


#define BLOCK_SIZE 1024


int cw_out_image_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst)	// ,struct cw_item * item) 
{
	cw_item_t * item = cw_itemstore_get(conn->outgoing,CW_ITEM_IMAGE_FILEHANDLE);
	if (!item) {
		cw_log(LOG_ERR,"Can't put element Image Data, no image filehandle found");
		return 0;
	}

	FILE *infile = item->data;


	int bytes = fread(dst+5,1,BLOCK_SIZE,infile);

	if (feof(infile)){
		if (ferror(infile)){
			cw_log(LOG_ERROR,"Aborting image data transfer: %s",strerror(errno));
			bytes=1;
			cw_put_byte(dst+4,5);
			bytes=0;

		}
		else{
			/* Last image block */
			cw_put_byte(dst+4,2); 

		}

	}
	else{
		cw_put_byte(dst+4,1);
	}
		

	return bytes+1 + cw_put_elem_hdr(dst,a->elem_id,bytes+1);

}

