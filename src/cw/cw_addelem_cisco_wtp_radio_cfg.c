#include "capwap.h"
#include "capwap_cisco.h"

int cw_addelem_cisco_wtp_radio_cfg(uint8_t*dst,struct radioinfo *ri)
{
	cw_put_byte(dst+10,ri->rid);
	cw_put_byte(dst+10+1,0);
	cw_put_word(dst+10+2,ri->occupancy_limit);
	cw_put_byte(dst+10+4,ri->cfp_period);
	cw_put_word(dst+10+5,ri->cfp_max_duration);

	/* XXX catch rmac shorter or longer than 6*/

	cw_put_bstr(dst+10+7,ri->rmac);	/* length MUST be 6 */
	
	cw_put_word(dst+10+13,ri->beacon_period);
	cw_put_data(dst+10+15,ri->country_str,3);
	cw_put_data(dst+10+18,ri->country_str2,3);

	cw_put_byte(dst+10+21,10); // gPeriod

	cw_put_dword(dst+10+22,0x3538);	// ?

	return 28+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_WTP_RADIO_CFG,28);
}

