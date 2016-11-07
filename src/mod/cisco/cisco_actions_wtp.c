/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "cw/cipwap.h"

#include "cw/action.h"
#include "cw/capwap_items.h"
#include "cw/strheap.h"
#include "cw/radio.h"
#include "cw/capwap_cisco.h"
#include "cw/capwap80211.h"
#include "cw/capwap80211_items.h"
#include "cw/lwapp_cisco.h"
#include "cisco_items.h"

#include "include/cipwap_items.h"

#include "mod_cisco.h"

#include "cisco.h"
#include "cisco80211.h"


static cw_action_in_t actions_in[] = {

	/* ----------------------------------------------------------------
	 * Discovery Response
	 */


	/* AC Descriptor - Discovery Response */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id  = CW_ELEM_AC_DESCRIPTOR,
		.item_id = CW_ITEM_AC_DESCRIPTOR, 
		.start  = cisco_in_ac_descriptor,
		.min_len = 12,
		.max_len = 8192,
		.mand = 1
	}
	,


	/* ----------------------------------------------------------------
	 * Join Response
	 */


	/* ECN Support - Join Response */
	{
		/* Make ECN Support non-mand */
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT,
	 	.start = cw_in_generic2, 
		.mand = 0, 
		.min_len = 1, 
		.max_len = 1
	}
	,

	/* AC Descriptor - Join Response */
	{
		/* Cisco's AC Descriptor */
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_AC_DESCRIPTOR,
		.item_id = CW_ITEM_AC_DESCRIPTOR, 
		.start  = cisco_in_ac_descriptor,
		.min_len = 12,
		.max_len = 8192,
		.mand = 1
	}
	,


	/* ----------------------------------------------------------------
	 * Configuration Update Request
	 */

	/* Location Data - Conf Update Req */
	{
		.capwap_state = CW_STATE_RUN,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id  = LW_ELEM_LOCATION_DATA,
		.item_id = CW_ITEM_LOCATION_DATA, 
		.start  = cw_in_generic2,
		.min_len = 0,
		.max_len = 1024,
		.mand = 0
	}
	,

	/* WTP Name - Conf Update Req */
	{
		.capwap_state = CW_STATE_RUN,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id  = CW_CISCO_RAD_NAME,
		.item_id = CW_ITEM_WTP_NAME, 
		.start  = cw_in_generic2,
		.min_len = 0,
		.max_len = 1024,
		.mand = 0
	}
	,


	/* WTP Name - Conf Update Req */
	{
		.capwap_state = CW_STATE_RUN,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id  = CW_CISCO_AP_GROUP_NAME,
		.item_id = CIPWAP_ITEM_WTP_GROUP_NAME, 
		.start  = cw_in_generic2,
		.min_len = 0,
		.max_len = 1024,
		.mand = 0
	}
	,



	/*  Radio Admin State - Config Update Request */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id = CW_ELEM_RADIO_ADMINISTRATIVE_STATE,
		.item_id = CW_RADIOITEM_ADMIN_STATE,
		.start = cisco_in_radio_administrative_state,
		.min_len=2,
		.max_len=2,
		.mand = 0
	}
	,

	/* AP Mode and Type */
	{

		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_AP_MODE_AND_TYPE,
		.item_id = CISCO_ITEM_AP_MODE_AND_TYPE,
		.start = cw_in_generic2
	}
	,

	/* Add WLAN */
	{

		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_ADD_WLAN,
//		.item_id = CISCO_ITEM_ADD_WLAN,
		.start = cisco_in_add_wlan
	}
	,


		
		

	/* Element Cisco 802.11 Radio Config - Config Update Resp */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_WTP_RADIO_CFG, 
		.start=cisco80211_in_wtp_radio_configuration, 
		.item_id = "cisco_radio_cfg", 
	}
	,



	/* ----------------------------------------------------------------
	 * Configuration Status Response
	 */


	/* Element Cisco 802.11 Radio Conffig - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_WTP_RADIO_CFG, 
		.start=cisco80211_in_wtp_radio_configuration, 
		.item_id = "cisco_radio_cfg", 
	}
	,

		
	/*  Radio Admin State - Config Status Response */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.elem_id = CW_ELEM_RADIO_ADMINISTRATIVE_STATE,
		.item_id = CW_RADIOITEM_ADMIN_STATE,
//		.start = cw_in_radio_generic,
		.start = cisco_in_radio_administrative_state,
		.mand = 1
	}
	,



	{
		/* Cisco's Vendor specific encapsulation
		 * of LWAPP elements */

		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_SPAM_VENDOR_SPECIFIC,
		.start = lw_in_vendor_specific,

	}
	,


	/* LWAPP Vendor spec Messages */

	/* MWAR Hash Value */
	{

		.proto = CW_ACTION_PROTO_LWAPP,
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.vendor_id = LW_VENDOR_ID_CISCO,
		.elem_id = LW_CISCO_MWAR_HASH_VALUE,
		.item_id = CIPWAP_ITEM_AC_HASH_VALUE,
		.start = cw_in_generic2, //cisco_in_telnet_ssh
	}

	,
	/* End of list */
	{0, 0}
};


static cw_action_out_t actions_out[]={

	/* ----------------------------------------------------------------
	 * Message Discover Request 
	 */


	/* WTP Descriptor - Discovery */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_DESCRIPTOR,
		.item_id = CW_ITEM_WTP_DESCRIPTOR, 
		.out = cisco_out_wtp_descriptor,
		.mand = 1
	}

	,

	/* Cisco RAD Name (WTP Name) - Discovery */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id  = CW_CISCO_RAD_NAME,
		.item_id = CW_ITEM_WTP_NAME, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,


	/* ----------------------------------------------------------------
	 * Message Join Rquest 
	 */

	/* CAPWAP Local IP Address - Join Request */
	{
		/* use draft 7 elem ids */
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS, 
		.out = cw_out_capwap_local_ip_address_7,
		.mand = 1
	}
	,

	/* WTP Descriptor - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_DESCRIPTOR,
		.item_id = CW_ITEM_WTP_DESCRIPTOR, 
		.out = cisco_out_wtp_descriptor,
		.mand = 1
	}
	,

	/* WTP Group Name (WTP Name) - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id  = CW_CISCO_AP_GROUP_NAME,
		.item_id = CIPWAP_ITEM_WTP_GROUP_NAME, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* WTP Board Data Options - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id  = CW_CISCO_BOARD_DATA_OPTIONS,
		.item_id = CW_ITEM_CISCO_BOARD_DATA_OPTIONS, 
		.out = cisco_out_board_data_options,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

		

	/* ECN Support - Join Request */
	{
		/* Cisco doesn't know ECN support, so we 
		   define no output method */
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT, 
		.mand = 0
	}
	,


	/* AP Mode and Type -  Configuration Status Request OUT */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO, 
		.elem_id = CW_CISCO_AP_MODE_AND_TYPE,
		.item_id = CISCO_ITEM_AP_MODE_AND_TYPE,
	 	.out = cw_out_generic, 
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	
	/* Cisco WTP Admin state -  OUT */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO, 
		.item_id = CISCO_ITEM_WTP_ADMIN_STATE,
	 	.out = cisco_out_wtp_administrative_state, 
	}
	,


	/* Cisco Admin state -  OUT */
/*	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO, 
//		.elem_id = CW_CISCO_AP_MODE_AND_TYPE,
		.item_id = CISCO_ITEM_WTP_ADMIN_STATE,
	 	.out = cisco_out_wtp_administrative_state, 
//		.get = cw_out_get_outgoming,
	}
	,

*/
	
	/* Cisco WTP Operational state -  OUT */
	{
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_REQUEST,
		.vendor_id = CW_VENDOR_ID_CISCO, 
//		.elem_id = CW_CISCO_AP_MODE_AND_TYPE,
		.item_id = CISCO_ITEM_WTP_OPER_STATE,
//	 	.out = cisco_out_wtp_operational_state, 
//		.get = cw_out_get_outgoming,
	}
	,




		
	/* Radio Operational State  - OUT */
	{
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_REQUEST,
		.elem_id = CW_ELEM_RADIO_OPERATIONAL_STATE,
		.item_id = CW_RADIOITEM_OPER_STATE,
	 	.out = cisco_out_radio_operational_state,
		.mand = 1
	}
	,




	{0,0}

};

static cw_action_in_t actions80211_in[] = {


	/* Radio Operational State - Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id= CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.elem_id = CW_ELEM_RADIO_OPERATIONAL_STATE,
		.item_id = CW_RADIOITEM_OPER_STATE,
//		.start = cw_in_radio_generic, //operational_state,
		.start = cisco_in_radio_operational_state, //operational_state,
		.min_len=3,
		.max_len=3,
		.mand = 0
	}
	,

	/* Radio Operational State - Update Req */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id= CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id = CW_ELEM_RADIO_OPERATIONAL_STATE,
		.item_id = CW_RADIOITEM_OPER_STATE,
//		.start = cw_in_radio_generic, //operational_state,
		.start = cisco_in_radio_operational_state, //operational_state,
		.min_len=3,
		.max_len=3,
		.mand = 0
	}
	,



};

static cw_action_out_t actions80211_out[]={

	/* ----------------------------------------------------------------
	 * Configuration Status Request
	 */
	{
		.vendor_id = CW_VENDOR_ID_CISCO,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = CW_RADIOITEM80211_WTP_RADIO_INFORMATION,
		.out = cisco_out_80211_wtp_radio_cfg,
		.get = cw_out_get_config,
		.mand = 1
	}
	,



	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST, 
		.item_id = CW_RADIO_SUPPORTED_RATES ,
	 	.elem_id = CW_ELEM80211_SUPPORTED_RATES, 
		.mand = 0,
	}



};




#include "cw/item.h"

static struct cw_itemdef _capwap_itemdefs[] = {

	{"wtp_name_cisco",CW_ITEM_NONE,MBAG_STR},

	/*	{"wtp_mac_type",CW_ITEM_NONE,MBAG_BYTE},
	{"discovery_type",CW_ITEM_NONE,MBAG_BYTE},
	{"wtp_frame_tunnel_mode",CW_ITEM_NONE,MBAG_BYTE},
*/	{CW_ITEM_NONE}

};


#include "../modload.h"

int cisco_register_actions_wtp(struct cw_actiondef *def)
{

	int rc;
	rc = cw_actionlist_in_register_actions(def->in, actions_in);
	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

//	intavltree_add(def->wbids, 0);



	return rc;
}



int cisco_register_actions80211_wtp(struct cw_actiondef *def)
{

	int rc;
	rc=0;
	rc = cw_actionlist_in_register_actions(def->in, actions80211_in);
	rc += cw_actionlist_out_register_actions(def->out, actions80211_out);
	rc += cw_itemdefheap_register(def->items, cisco_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, cisco_radioitemdefs);

/*	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);
*/


	return rc;
}


