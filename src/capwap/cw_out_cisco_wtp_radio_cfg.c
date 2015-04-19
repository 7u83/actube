#include "capwap.h"
#include "capwap_cisco.h"

#include "itemstore.h"

int cw_put_cisco_wtp_radio_cfg(uint8_t *dst, int rid, cw_itemstore radio)
{

	cw_put_byte(dst,rid);

	cw_put_byte(dst+1,0); //?
	cw_put_word(dst+2,cw_istore_get_word(radio,CW_RADIO_OCCUPANCY_LIMIT);
	cw_put_byte(dst+4,cw_istore_get_byte(radio,CW_RADIO_CFP_PERIOD));

	cw_put_word(dst+5,cw_istore_get_word(radio,CW_RADIO_CFP_MAX_DURATION,200);

	/* XXX catch rmac shorter or longer than 6*/

	cw_put_bstr(dst+7,cw_istore_get_datari->rmac);	/* length MUST be 6 */
	
	cw_put_word(dst+13,200); // beacon period
	cw_put_data(dst+15,cw_itemstore_get_datari->country_str,3);
	cw_put_data(dst+18,ri->country_str2,3);

	cw_put_byte(dst+10+21,10); // gPeriod

	cw_put_dword(dst+10+22,0x3538);	// ?

	return 28; //+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_WTP_RADIO_CFG,28);
}




int cw_out_cisco_wtp_radio_cfg(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	uint8_t *d = dst;


	return 28+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_WTP_RADIO_CFG,28);
}

