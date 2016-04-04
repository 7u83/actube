
#include "cw/mbag.h"
#include "cw/action.h"
#include "cw/dbg.h"
#include "cw/cw.h"

#include "cisco_items.h"
#include "include/cipwap_items.h"
#include "cw/capwap80211_items.h"

//int mbag_get_upd(mbag_t b, mbag_t b_upd, const char *id,
//		 uint8_t * dst, struct mbag_typedef * deftype, uint8_t * def, int deflen);


int mbag_get_upd(mbag_t b, mbag_t b_upd, const char *id, uint8_t *dst, int *found);

int cisco80211_out_wtp_radio_configuration(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

cw_dbg(DBG_X,"The update beginns ***************************************************************");
	int count=0;
	int n;
	uint8_t *d = dst+10;

	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *r = mavliter_get(&it);
		mbag_t radio_upd = r->data;
		mbag_t radio = mbag_i_get_mbag(conn->radios,r->iid,NULL);

		radio = radio_upd;

		d+=cw_put_byte(d,r->iid);

//		mbag_t radio = mbag_i_get(conn->radios,radio_upd->data->iid);

		n  = mbag_get_upd(radio,radio_upd,CISCO_RADIOITEM80211_CFG_TYPE,d,&count);
		d += n==-1 ? cw_put_byte(dst,0) : n;
		n  = mbag_get_upd(radio,radio_upd,CIPWAP_RADIOITEM80211_OCCUPANCY_LIMIT,d,&count);
		d += n==-1 ? cw_put_word(dst,100) : n;
		n  = mbag_get_upd(radio,radio_upd,CIPWAP_RADIOITEM80211_CFP_PERIOD,d,&count);
		d += n==-1 ? cw_put_byte(dst,4) : n;
		n  = mbag_get_upd(radio,radio_upd,CIPWAP_RADIOITEM80211_CFP_MAXIMUM_DURATION,d,&count);
		d += n==-1 ? cw_put_word(dst,60) : n;
		
		n  = mbag_get_upd(radio,radio_upd,CW_RADIOITEM80211_BSSID,d,&count);
		if (n==-1){
			char defbssid[6]={1,2,3,4,5,6};
			memcpy(d,defbssid,6);
			d+=6;
		}
		else
			d+=n;

		n  = mbag_get_upd(radio,radio_upd,CW_RADIOITEM80211_BEACON_PERIOD,d,&count);
		d += n==-1 ? cw_put_word(dst,100) : n;



		int dcount = 0;
		n  = mbag_get_upd(radio,radio_upd,CISCO_RADIOITEM80211_COUNTRY_STR1,d,&dcount);
		if (!dcount){
			n  = mbag_get_upd(radio,radio_upd,CW_RADIOITEM80211_COUNTRY_STRING,d,&count);
			d += n==-1 ? cw_put_data(d,(uint8_t*)"DE ",3) : n;
		}
		else
			d+=n;

		dcount=0;
		n  = mbag_get_upd(radio,radio_upd,CISCO_RADIOITEM80211_COUNTRY_STR2,d,&dcount);
		if (!dcount){
			n  = mbag_get_upd(radio,radio_upd,CW_RADIOITEM80211_COUNTRY_STRING,d,&count);
			d += n==-1 ? cw_put_data(d,(uint8_t*)"DE ",3) : n;
		}
		else
			d+=n;

		count +=dcount;


		d+=cw_put_byte(d,10);
		d+=cw_put_word(d,1);
		d+=cw_put_word(d,0);
		d+=cw_put_word(d,177<<8);





	}

	if (!count){
		cw_dbg(DBG_X,"Return 0, because no item was in radio");
		return 0;
	}

	cw_dbg(DBG_X,"Yupp we do!");
	int l = d-dst-10;
	return l + cw_put_elem_vendor_hdr(dst, a->vendor_id, a->elem_id, l);


	/*


	mbag_set_word(r,CW_RADIOITEM80211_BEACON_PERIOD,cw_get_word(data+13));
	mbag_set_bstr16n(r,CW_RADIOITEM80211_COUNTRY_STRING,data+15,3);
	mbag_set_bstr16n(r,CISCO_RADIOITEM80211_COUNTRY_STR1,data+15,3);
	mbag_set_bstr16n(r,CISCO_RADIOITEM80211_COUNTRY_STR2,data+18,3);

*/

//	mbag_set_byte(r,CISCO_RADIOITEM80211_CFG_TYPE,cw_get_byte(data+1));
		
	


}
