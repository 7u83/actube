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

#include "mod_cisco.h"
#include "cisco.h"

static cw_action_in_t actions_in[] = {


	

	/* End of list */
	{0, 0}
};


static cw_action_out_t actions_out[]={





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


