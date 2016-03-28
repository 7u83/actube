#include "cw.h"
#include "capwap80211.h"
#include "radio.h"


int cw_put_elem_80211_supported_rates(uint8_t*dst,int radio_id,mbag_t radio)
{

	struct mbag_item * sr = mbag_get(radio,CW_RADIO_SUPPORTED_RATES);
	if (!sr)
		return 0;

	int n = cw_put_byte(dst+4,radio_id);

	n+=cw_put_mbag_item(dst+5,sr);
	return 5 + cw_put_elem_hdr(dst,CW_ELEM80211_SUPPORTED_RATES,5);
}


int cw_out_80211_supported_rates(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	int l=0;
	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
		l+=cw_put_elem_80211_supported_rates(dst+l,i->iid,i->data);
	}
	return l;
}


