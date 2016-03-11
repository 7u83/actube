#include <stdio.h>
#include <errno.h>

#include "cw.h"
#include "capwap_items.h"

#include "log.h"
#include "dbg.h"


#include "sock.h"








int cw_out_image_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	mbag_item_t * item = mbag_get(conn->outgoing,CW_ITEM_IMAGE_FILEHANDLE);
	if (!item) {
		cw_log(LOG_ERR,"Can't put element Image Data, no image filehandle found");
		return 0;
	}

	FILE *infile = item->data;
	if (infile==NULL){
		cw_log(LOG_ERR,"Image Data Request infile = NULL");
		return 0;
	}
	
	int bytes=0;
	switch ( conn->capwap_mode_out){
		case CW_MODE_CISCO:
			bytes = lw_put_image_data(dst+4,infile);
			if ( bytes != LW_BLOCKSIZE_IMAGE_DATA + 3) {
       		        	avltree_del(conn->outgoing, item);
			}
			break;
		default:
			bytes = cw_put_image_data(dst+4,infile);
			if (dst[4] != 1){
       		        	avltree_del(conn->outgoing, item);
			}
	}

	if ( ferror(infile)){
		cw_log(LOG_ERROR,"Aborting image data transfer: %s",strerror(errno));
	}

	return bytes + cw_put_elem_hdr(dst,a->elem_id,bytes);
}

