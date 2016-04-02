
#include "cw/mbag.h"
#include "cw/action.h"
#include "cw/cw.h"
#include "cw/dbg.h"

#include "include/cipwap_items.h"
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


	
	return 1;

}

