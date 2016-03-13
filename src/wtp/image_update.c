#include <errno.h>
#include "wtp.h"

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/capwap.h"
#include "cw/capwap_items.h"
#include "cw/vendors.h"


int image_update()
{
	struct conn *conn = get_conn();
	if (conn->capwap_state != CW_STATE_CONFIGURE) {
		cw_log(LOG_ERR, "Current state not image update");
		return 0;
	}


	const char *ii = "/c1130";
	mbag_set_bstrv(conn->outgoing, CW_ITEM_IMAGE_IDENTIFIER,
				   CW_VENDOR_ID_CISCO, (uint8_t *) ii, strlen(ii));


	int rc = cw_send_request(conn, CW_MSG_IMAGE_DATA_REQUEST);

	if (rc < 0) {
	}

	if (rc != 0) {
		cw_log(LOG_ERR, "AC rejected Image Data Request with code: %d - %s", rc,
		       cw_strresult(rc));
		return 0;
	}

	cw_dbg(DBG_INFO,"Ready to receive image ...");

	conn->capwap_state=CW_STATE_IMAGE_DATA;
	rc=-11;
        while (conn->capwap_state == CW_STATE_IMAGE_DATA) {
                rc = cw_read_messages(conn);
                if (rc < 0) {
                        if (errno != EAGAIN) 
				break;
                }
        }


	printf("RC: %d %s\n",rc,strerror(errno));
	




	return 1;
}
