#include "capwap.h"
#include "capwap_cisco.h"

#include "mbag.h"
#include "radio.h"
#include "log.h"
#include "dbg.h"

int cw_put_cisco_wtp_radio_cfg(uint8_t *dst, int rid, mbag_t radio)
{

	cw_put_byte(dst,rid);

	cw_put_byte(dst+1,0); //?
	cw_put_word(dst+2,mbag_get_word(radio,CW_RADIO_OCCUPANCY_LIMIT,12));
	cw_put_byte(dst+4,mbag_get_byte(radio,CW_RADIO_CFP_PERIOD,8));

	cw_put_word(dst+5,mbag_get_word(radio,CW_RADIO_CFP_MAX_DURATION,200));


	bstr_t	grmac = mbag_get_bstr(radio,CW_RADIO_BSSID,NULL);

//printf("GRMAC: %d\n",bstr_len(grmac));
	if ( grmac) {
		if (bstr_len(grmac)!=6){
			cw_log(LOG_ERR,"Wrong bssid size");
exit(0);
			grmac =NULL;
		}
	}

	bstr_t rmac;

	if (!grmac){
		uint8_t defrmac[]={0,0,0,0,0,0};
		rmac = bstr_create(defrmac,6);
	}
	else
		rmac = grmac;
		

	cw_put_bstr(dst+7,rmac);

	if ( !grmac )
		free(rmac);
	
	cw_put_word(dst+13,0); // beacon period


	cw_put_data(dst+15,mbag_get_raw(radio,CW_RADIO_COUNTRY_STRING,"DE "),3);
	cw_put_data(dst+18,mbag_get_raw(radio,CW_RADIO_COUNTRY_STRING,"DE "),3);

	cw_put_byte(dst+21,10); // gPeriod

	cw_put_dword(dst+22,0x3538);	// ?

	cw_put_word(dst+26,0);

	return 26+2; //+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_WTP_RADIO_CFG,28);
}



int cw_put_elem_cisco_ap_regulatory_domain(uint8_t *dst,int rid, mbag_t radio)
{
	//int l=0;
	uint8_t *d=dst+10;
	
	d+=cw_put_byte(d,rid);		/* Band ID */
	d+=cw_put_byte(d,1);		/* Set True/False */
	d+=cw_put_byte(d,rid);		/* Slot ID */
	d+=cw_put_word(d,mbag_get_word(radio,CW_RADIO_REG_DOMAIN,1));

	return 5 + cw_put_elem_vendor_hdr(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_REGULATORY_DOMAIN, 5);
	

}



int cw_put_elem_cisco_radio_cfg(uint8_t * dst,int rid, mbag_t radio)
{
	int l = cw_put_cisco_wtp_radio_cfg(dst+10,rid,radio);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_WTP_RADIO_CFG,l);
}

int cw_out_cisco_wtp_radio_cfg(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	int l=0;
	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
//		l+=cw_put_elem_radio_info(dst+l,i->id,i->data);
		l+=cw_put_elem_cisco_radio_cfg(dst+l,i->iid,i->data);
		l+=cw_put_elem_cisco_ap_regulatory_domain(dst+l,i->iid,i->data);

	}
	return l;







//	MAVLITER_DEFINE
//	int l = cw_out_cisco_wtp_radio_cfg_(conn,a,dst,0);

//	return l+cw_out_cisco_wtp_radio_cfg_(conn,a,dst+l,1);
}






int cw_in_cisco_radio_cfg(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	int rid = cw_get_byte(data);
	mbag_t radio = mbag_i_get_mbag(conn->radios,rid,NULL);
	if ( !radio){
		cw_dbg(DBG_ELEM_ERR,"Radio ID %d not defined",rid);
		return 0;
	}

	
//	printf("Here we are %d\n",rid);
	return 0;
}


int cw_radio_cisco_set_state(struct conn * conn, uint8_t *data, int len, int cause)
{

	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);
	if (rid != 255)
		return cw_radio_set_admin_state(conn->radios,rid,state,cause);


	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		mbag_item_t *i = mavliter_get(&it);
		cw_radio_set_admin_state(conn->radios,i->iid,state,cause);

	}
	return 1;	

}


int cw_in_cisco_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	return cw_radio_cisco_set_state(conn,data,len,-1);	
}


int cw_in_cisco_radio_administrative_state_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	return cw_radio_cisco_set_state(conn,data,len,3);	
}


