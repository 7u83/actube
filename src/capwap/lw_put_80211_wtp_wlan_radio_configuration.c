
#include "lwapp.h"

int lw_put_80211_wtp_wlan_radio_configuration(uint8_t*dst,struct radioinfo *ri)
{
	lw_put_byte(dst,ri->rid);
	lw_put_byte(dst+1,0);
	lw_put_word(dst+2,ri->occupancy_limit);
	lw_put_byte(dst+4,ri->cfp_period);
	lw_put_word(dst+5,ri->cfp_max_duration);

	/* XXX catch rmac shorter or longer than 6*/

	lw_put_bstr(dst+7,ri->rmac);	/* length MUST be 6 */
	
	lw_put_word(dst+13,ri->beacon_period);
	lw_put_byte(dst+15,ri->dtim_period);
	lw_put_data(dst+16,ri->country_str,4);
	lw_put_byte(dst+20,ri->max_bssid);

	/* XXX not LWAP conform */
	lw_put_data(dst+21,(uint8_t*)"DEAU990",7);

	return 21+7;	
}

