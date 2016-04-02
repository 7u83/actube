#ifndef __CW_RADIO_H
#define __CW_RADIO_H

#include "mbag.h"
#include "item.h"
#include "conn.h"




//extern const char CW_RADIO_ADMIN_STATE[];
//extern const char CW_RADIO_OPER_STATE[];

extern const char CW_RADIO_SUPPORTED_RATES[];


extern const char CW_RADIO_TYPE[];
extern const char CW_RADIO_REG_DOMAIN[];
extern const char CW_RADIO_BSSID[];
extern const char CW_RADIO_SHORT_PREAMBLE[];
extern const char CW_RADIO_COUNTRY_STRING[];
extern const char CW_RADIO_DECRYPTION_ERROR_REPORT_PERIOD[];

/* Cisco */

extern const char CW_RADIO_OCCUPANCY_LIMIT[];
extern const char CW_RADIO_CFP_PERIOD[];
extern const char CW_RADIO_CFP_MAX_DURATION[];
	

/*
enum radiodata{
	CW_RADIO_ADMIN_STATE=10000,
	CW_RADIO_OPER_STATE,
	CW_RADIO_TYPE,
	CW_RADIO_REG_DOMAIN,
	CW_RADIO_BSSID,
	CW_RADIO_SHORT_PREAMBLE,
	CW_RADIO_COUNTRY_STRING,


	CW_RADIO_OCCUPANCY_LIMIT,
	CW_RADIO_CFP_PERIOD,
	CW_RADIO_CFP_MAX_DURATION
	


}[];
*/



int cw_radio_update_oper_states(mbag_t radios,int cause);
extern struct cw_itemdef capwap_radioitemdefs[];
extern int cw_radio_set_all_admin_states(mbag_t radios,int state, int cause);

int cw_out_80211_supported_rates(struct conn *conn, struct cw_action_out *a, uint8_t * dst);



#endif
