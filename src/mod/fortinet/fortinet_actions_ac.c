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


#include "cw/capwap.h"
#include "cw/cipwap.h"

#include "cw/action.h"
#include "cw/capwap_items.h"
#include "cw/strheap.h"
#include "cw/radio.h"
#include "cw/capwap_cisco.h"
#include "cw/capwap80211.h"

#include "mod_fortinet.h"
#include "fortinet.h"

static cw_action_in_t actions_in[] = {


	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* Message Discovery Request */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_REQUEST,
		.end = cw_in_check_disc_req
	}
	,
#if 0
	/* Element WTP Descriptor */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_WTP_DESCRIPTOR,
	 	.start = cisco_in_wtp_descriptor, 
		.item_id = "wtp_descriptor", 
		.mand = 1, 
	}
	,

	/* Element Cisco RAD Name */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id = CW_CISCO_RAD_NAME, 
		.start=cw_in_generic2, 
		.item_id = "wtp_name", 
		.min_len=1,
		.max_len=512,
		.mand=1
	}
	,





	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* WTP Descriptor - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_DESCRIPTOR,
	 	.start = cisco_in_wtp_descriptor, 
		.item_id = "wtp_descriptor", 
		.mand = 1, 
	}
	,

	/* Session ID - Join Request */
	{
		/* Cisco uses 4 byte session ids */
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_SESSION_ID,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_SESSION_ID, 
		.mand = 1, 
		.min_len = 4, 
		.max_len = 16
	}
	,

	/* Local IPv4 Address - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_IPV4_IP_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS,
	 	.start = cw_in_capwap_local_ipv4_address, 
		.mand = 1,
		.min_len = 4, 
		.max_len = 4
	}
	,

	/* Local IPv6 Address - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_IPV6_IP_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS,
	 	.start = cw_in_capwap_local_ipv4_address, 
		.mand = 1,
		.min_len = 16, 
		.max_len = 16
	}
	,

	/* ECN Support - Join Request */
	{
		/* Cisco (using draft 7) does nothing know 
		 * about ECN support, so make it non-mandatory */
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT,
	 	.start = cw_in_generic2, 
		.mand = 0, 
		.min_len = 1, 
		.max_len = 1
	}
	,


	/* --------------------------------------------------------
	 * Configuration Status Request 
	 */

	/* AC Name - Config Status Request */	
	{
		/* We have to deal with zero-length strings */
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.elem_id = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME,
		.start = cw_in_generic2,
		.min_len = 0,
		.max_len = 512,
		.mand = 1

	}
	,


#endif
	/* End of list */
	{0, 0}
};


static cw_action_out_t actions_out[]={

	/* --------------------------------------------------------
	 * Discovery Response 
	 */

#if 0
	/* Cisco AP Timesync - Discovery Request
	 * Important to get the WTP a DTLS connection established
	 */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.item_id = CW_ITEM_AC_TIMESTAMP, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.elem_id  = CW_CISCO_AP_TIMESYNC,
		.out = cisco_out_ap_timesync,
		.mand = 1
	}
	,
	/* AC Descriptor - Discovery Response */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.item_id = CW_ITEM_AC_DESCRIPTOR,
		.elem_id = CW_ELEM_AC_DESCRIPTOR, 
		.out = cisco_out_ac_descriptor,
		.mand = 1
	}
	,


	/* AC Descriptor - Join Response */
	{
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.item_id = CW_ITEM_AC_DESCRIPTOR,
		.elem_id = CW_ELEM_AC_DESCRIPTOR, 
		.out = cisco_out_ac_descriptor,
		.mand = 1
	}
	,



	/* ECN Support - Join Response */
	{
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT
	}
	,

	/* --------------------------------------------------------
	 * Echo Response 
	 */
	/* AP Time Sync - Echo Response */
	{
		.msg_id = CW_MSG_ECHO_RESPONSE, 
		.vendor_id = CW_VENDOR_ID_CISCO,
		.item_id = CW_ITEM_AC_TIMESTAMP, 
		.elem_id  = CW_CISCO_AP_TIMESYNC,
		.out = cisco_out_ap_timesync,
		.mand = 1
	}
	,
#endif
	{0,0}

};

static cw_action_in_t actions80211_in[] = {
	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* 802.11 Radio Inmformation - Discovery Request */
	{
		/* Cisco doe't sned this message element in discovery request,
		   so make it non-mandatory */

		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = "radio_information",
	 	.start = cw_in_radio_generic, 
		.mand = 0, 
		.min_len = 5, 
		.max_len = 5
	}
	,


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

int fortinet_register_actions_ac(struct cw_actiondef *def)
{

	int rc;
	rc = cw_actionlist_in_register_actions(def->in, actions_in);
	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);



	return rc;
}



int fortinet_register_actions80211_ac(struct cw_actiondef *def)
{

	int rc;
	rc=0;
	rc = cw_actionlist_in_register_actions(def->in, actions80211_in);
/*	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);
*/


	return rc;
}


