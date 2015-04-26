
#include "capwap.h"
#include "capwap_items.h"
#include "capwap_80211.h"
#include "radio.h"

#include "dbg.h"
#include "log.h"

#include "mbag.h"



int cw_put_elem_radio_info(uint8_t*dst,int radio_id,mbag_t radio)
{
	cw_put_byte(dst+4,radio_id);
	cw_put_dword(dst+5,mbag_get_dword(radio,CW_RADIO_TYPE,0));
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
		l+=cw_put_elem_radio_info(dst+l,i->id,i->data);

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
		l+=cw_put_elem_radio_administrative_state(dst+l,i->id,i->data);

	}
	return l;
}



static int set_radio_admin_state(mbag_t radios,int rid,int state)
{

	mbag_t radio = mbag_get_mbag(radios,rid,NULL);
	if (!radio){
		cw_dbg(DBG_ELEM_ERR,"Can't set radio administrative state for radio %d - radio does't exists",rid);
		return 0;	
	}

	mbag_set_byte(radio,CW_RADIO_ADMIN_STATE,state);
	
	return 1;
}

int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);
	return set_radio_admin_state(conn->radios,rid,state);
}


int cw_in_cisco_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);
	if (rid != 255)
		return set_radio_admin_state(conn->radios,rid,state);


	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		mbag_item_t *i = mavliter_get(&it);
		mbag_set_byte(i->data,CW_RADIO_ADMIN_STATE,state);
	}
	return 1;	
}
