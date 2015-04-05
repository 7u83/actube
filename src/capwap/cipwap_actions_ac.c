
#include "capwap.h"
#include "capwap_cisco.h"
#include "capwap_actions.h"
#include "cipwap.h"

/**
 *@file
 *brief CIPWAP Actions
 */



cw_action_in_t cipwap_actions_ac_in[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Request IN
	 */

	{CW_VENDOR_ID_CISCO, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_CISCO_RAD_NAME, 
	cw_in_generic, 0, CW_ITEMTYPE_STR,CW_ITEM_WTP_NAME,1,512}
	,


	{CW_VENDOR_ID_CISCO, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_CISCO_AP_GROUP_NAME, 
	cw_in_generic, 0, CW_ITEMTYPE_STR,CW_ITEM_WTP_GROUP_NAME,1,512}
	,



	{0,0,0}
};

extern struct cw_item *cw_get_local(struct conn *conn, struct cw_action_out *a);

cw_action_out_t cipwap_actions_ac_out[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Response OUT
	 */


	/* AC Descriptor (Cisco) */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0 ,
	CW_ELEM_AC_DESCRIPTOR, cw_out_cisco_ac_descriptor, 0}
	,

	/* Cisco AP Timesync - Important to get the WTP a DTLS 
	   connection stablished*/
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_TIMESTAMP, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_TIMESYNC, cw_out_cisco_ap_timesync, 0}
	,


	{0,0,0}
	
};

/**
 * Register CiPWAP actions
 */ 
int cw_register_actions_cipwap_ac(struct cw_actiondef *def)
{
	cw_register_actions_capwap_ac(def);

	cw_actionlist_in_register_actions(def->in, cipwap_actions_ac_in);
	cw_actionlist_out_register_actions(def->out, cipwap_actions_ac_out);
	cw_strheap_register_strings(def->strelem,cipwap_strings_elem);

	return 1;
}

