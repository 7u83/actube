
#include <stdlib.h>
#include <string.h>

#include "capwap.h"
#include "bstr.h"


static int add_board_data_subelem(uint8_t *msg,int type, bstr_t data)
{
	if (!data)
		return 0;


	int l = bstr_len(data);
	*((uint32_t *) (msg)) = htonl(type << 16 | l);
	memcpy(msg + 4, bstr_data(data), l);
	return l+4;

}


void cwmsg_addelem_wtp_board_data(struct cwmsg *cwmsg, struct wtpinfo *wtpinfo)
{
	uint8_t msg[1024];

	/* vendor identifier */
	*((uint32_t *) msg) = htonl(wtpinfo->vendor_id);

	int len = 4;

	/* mandatory sub-elements */


	len += add_board_data_subelem(msg+len,CWBOARDDATA_MODELNO,wtpinfo->model_no);
	
	len += add_board_data_subelem(msg+len,CWBOARDDATA_SERIALNO,wtpinfo->serial_no);


	/* other sub-elements */

	len += add_board_data_subelem(msg+len,CWBOARDDATA_BOARDID,wtpinfo->board_id);

	len += add_board_data_subelem(msg+len,CWBOARDDATA_REVISION,wtpinfo->board_revision);


	if (wtpinfo->macaddress) {
		*((uint32_t *) (msg + len)) =
		    htonl(CWBOARDDATA_MACADDRESS << 16 | wtpinfo->macaddress_len);
		memcpy(msg + len + 4, wtpinfo->macaddress, wtpinfo->macaddress_len);
		len += wtpinfo->macaddress_len + 4;
	}

	cwmsg_addelem(cwmsg, CWMSGELEM_WTP_BOARD_DATA, msg, len);
}
