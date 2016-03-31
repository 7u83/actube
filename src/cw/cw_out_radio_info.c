
#include "cw.h"
#include "capwap_items.h"
#include "capwap80211.h"
#include "capwap80211_items.h"

#include "radio.h"

#include "dbg.h"
#include "log.h"

#include "mbag.h"



int cw_put_elem_radio_info(uint8_t*dst,int radio_id,mbag_t radio)
{
	cw_put_byte(dst+4,radio_id);
	cw_put_dword(dst+5,mbag_get_dword(radio,CW_ITEM80211_WTP_RADIO_INFORMATION,0));
	return 5 + cw_put_elem_hdr(dst,CW_ELEM80211_WTP_RADIO_INFORMATION,5);

}


int cw_put_elem_radio_administrative_state(uint8_t *dst,int radio_id,mbag_t radio)
{
	cw_put_byte(dst+4,radio_id);
	cw_put_byte(dst+5,mbag_get_byte(radio,CW_RADIO_ADMIN_STATE,9));
	return 2 + cw_put_elem_hdr(dst,CW_ELEM_RADIO_ADMINISTRATIVE_STATE,2);
}

int cw_out_radio_infos(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	int l=0;
	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
		l+=cw_put_elem_radio_info(dst+l,i->iid,i->data);

	}
	return l;
}


int cw_out_radio_administrative_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	int l=0;
	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
		l+=cw_put_elem_radio_administrative_state(dst+l,i->iid,i->data);

	}
	return l;
}




