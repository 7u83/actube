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

#include "mod_cisco.h"
#include "cisco.h"

#include "cisco80211.h"


static cw_action_in_t actions_in[] = {

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



	/* ECN Support - Join Response */
	{
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

	

	/* End of list */
	{0, 0}
};


static cw_action_out_t actions_out[]={


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
		.item_id = CW_ITEM_WTP_GROUP_NAME, 
		.out = cw_out_generic,
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


	/* ----------------------------------------------------------------
	 * Configuration Status Request
	 */
	{
		.vendor_id = CW_VENDOR_ID_CISCO,
		.item_id = CW_ITEM_RADIO_CFG,
		.out = cisco_out_80211_wtp_radio_cfg,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

//	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_RADIO_CFG, CW_VENDOR_ID_CISCO,
//	CW_CISCO_AP_GROUP_NAME, NULL,cw_out_cisco_wtp_radio_cfg, cw_out_get_config, 1}
//	,



	{0,0}

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
//	rc = cw_actionlist_in_register_actions(def->in, actions80211_in);
/*	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);
*/


	return rc;
}


