#include <stdio.h>
#include <errno.h>

#include "capwap.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"

#include <unistd.h> //Tube

int cw_in_check_img_data_req_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{

	mbag_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);	
		conn->capwap_state=CW_STATE_IMAGE_DATA;
//usleep(100000);
	return 0;

	return CW_RESULT_IMAGE_DATA_ERROR;
	return 0;


	/* Check for mandatory elements */
	cw_action_in_t * mlist[60];
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n) {
		cw_dbg_missing_mand(DBG_ELEM,conn,mlist,n,a);
		conn->capwap_state=CW_STATE_JOIN;
		return CW_RESULT_MISSING_MAND_ELEM;
	}

	
	struct mbag_item *i = mbag_get(conn->incomming,CW_ITEM_IMAGE_IDENTIFIER);
	if (i) {
		uint32_t vendor_id = bstrv_get_vendor_id(i->data);

		const char * image_dir;
		image_dir = mbag_get_str(conn->local,CW_ITEM_AC_IMAGE_DIR,"./img");

		char * image_filename = malloc(6+bstrv_len(i->data)+1+strlen(image_dir));
		if (!image_filename) 
			return CW_RESULT_IMAGE_DATA_ERROR;

		sprintf(image_filename,"%s%04X/%s",image_dir,vendor_id,bstrv_data(i->data));


		FILE *infile = fopen(image_filename,"rb");
		if (!infile){
			cw_log(LOG_WARNING,"Can't open image file: %s - %s - requestet by WTP",
				image_filename,strerror(errno));
			free(image_filename);
			return CW_RESULT_IMAGE_DATA_ERROR;
		}

		mbag_set_str(conn->outgoing,CW_ITEM_IMAGE_FILENAME,image_filename);
		mbag_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);	
		conn->capwap_state=CW_STATE_IMAGE_DATA;
		return 0;
	}

	

	return CW_RESULT_IMAGE_DATA_ERROR;

}
