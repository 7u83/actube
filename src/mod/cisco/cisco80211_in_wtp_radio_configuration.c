
#include "cw/mbag.h"
#include "cw/action.h"
#include "cw/cw.h"
#include "cw/dbg.h"

#include "include/cipwap_items.h"
#include "cw/capwap80211_items.h"
#include "cisco_items.h"


int cisco80211_in_wtp_radio_configuration(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	mbag_t r = mbag_i_get_mbag(conn->radios,rid,NULL);
	if (!r){
		cw_dbg(DBG_ELEM_ERR,"Radio not found %d",r);
		return 0;
	}

	mbag_set_byte(r,CISCO_RADIOITEM80211_CFG_TYPE,cw_get_byte(data+1));
	mbag_set_word(r,CIPWAP_RADIOITEM80211_OCCUPANCY_LIMIT,cw_get_word(data+2));
	mbag_set_byte(r,CIPWAP_RADIOITEM80211_CFP_PERIOD,cw_get_byte(data+4));
	mbag_set_word(r,CIPWAP_RADIOITEM80211_CFP_MAXIMUM_DURATION,cw_get_word(data+5));

	struct mbag_item * bssid = MBAG_DATA->get(data+7,6);
	bssid->id=CW_RADIOITEM80211_BSSID;
	mbag_set(r,bssid);

	mbag_set_word(r,CW_RADIOITEM80211_BEACON_PERIOD,cw_get_word(data+13));
	mbag_set_bstr16n(r,CW_RADIOITEM80211_COUNTRY_STRING,data+15,3);
	mbag_set_bstr16n(r,CISCO_RADIOITEM80211_COUNTRY_STR1,data+15,3);
	mbag_set_bstr16n(r,CISCO_RADIOITEM80211_COUNTRY_STR2,data+18,3);

	
	return 1;

}

