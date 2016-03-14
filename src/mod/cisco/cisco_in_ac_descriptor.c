
#include "cw/cw.h"
#include "cw/capwap_items.h"
#include "cw/dbg.h"
#include "cw/mbag.h"
#include "cw/vendors.h"

int cisco_in_ac_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{
	static struct cw_descriptor_subelem_def allowed[] = {
		{CW_VENDOR_ID_CISCO,0, CW_ITEM_AC_HARDWARE_VERSION, 1024,1},
		{CW_VENDOR_ID_CISCO,1, CW_ITEM_AC_SOFTWARE_VERSION, 1024,1},
		{0,0, NULL,0, 0}
	};


	return cw_read_ac_descriptor(conn->config,data,len,allowed);

}
