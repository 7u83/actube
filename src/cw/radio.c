


#include "radio.h"
#include "dbg.h"



const char CW_RADIO_ADMIN_STATE[]="admin_state";
const char CW_RADIO_OPER_STATE[]="oper_state";
const char CW_RADIO_TYPE[]="radio_type";
const char CW_RADIO_REG_DOMAIN[]="reg_domain";
const char CW_RADIO_BSSID[]="bssid";
const char CW_RADIO_SHORT_PREAMBLE[]="short_preamble";
const char CW_RADIO_COUNTRY_STRING[]="country_string";
const char CW_RADIO_DECRYPTION_ERROR_REPORT_PERIOD[]="decryption_error_report_period";


const char CW_RADIO_SUPPORTED_RATES[]="supported_rates";



/* Cisco */

const char CW_RADIO_OCCUPANCY_LIMIT[]="occupancy_limit";
const char CW_RADIO_CFP_PERIOD[]="cfp_period";
const char CW_RADIO_CFP_MAX_DURATION[]="cfp_max_duration";
	

struct cw_itemdef capwap_radioitemdefs[] = {
{CW_RADIO_ADMIN_STATE,CW_ITEM_NONE,MBAG_BYTE},
{CW_RADIO_TYPE,CW_ITEM_NONE,MBAG_DWORD},
{CW_RADIO_DECRYPTION_ERROR_REPORT_PERIOD,CW_ITEM_NONE,MBAG_WORD},

{CW_RADIO_BSSID,CW_ITEM_NONE,MBAG_BSTR16},
{CW_RADIO_SUPPORTED_RATES,CW_ITEM_NONE,MBAG_DATA},

{CW_ITEM_NONE}
};


/** 
 * Update all radio operational states.
 * @param radios definition of radios
 * @param cause cause to set
 */ 
int cw_radio_update_oper_states(mbag_t radios,int cause)
{
	MAVLITER_DEFINE(it,radios);
	mavliter_foreach(&it){
		mbag_item_t * i = mavliter_get(&it);
		
		/* Get admin state of the radio, set disabled (2) if admin state is
		   not defined */
		int admin_state = mbag_get_byte(i->data,CW_RADIO_ADMIN_STATE,2);
		mbag_set_word(i->data,CW_RADIO_OPER_STATE,(admin_state<<8) | cause );

	}
	return 1;
}






int cw_radio_set_all_admin_states(mbag_t radios,int state, int cause)
{
	MAVLITER_DEFINE(it,radios);
	mavliter_foreach(&it){
		mbag_item_t *i = mavliter_get(&it);
		cw_radio_set_admin_state(radios,i->iid,state,cause);

	}
	return 1;	
}



