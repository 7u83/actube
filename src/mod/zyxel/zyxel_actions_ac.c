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
#include "cw/capwap80211.h"
#include "cw/capwap80211_items.h"

#include "cw/cw_80211.h"

#include "include/capwap_actions.h"



#include "include/cipwap_items.h"


static cw_action_in_t actions_in[] = {



	/* End of list */
	{0, 0}
};


static cw_action_out_t actions_out[]={
	{0,0}

};

static cw_action_in_t actions80211_in[] = {

	{0,0}

};




static cw_action_out_t actions80211_out[]={



	{0,0}
};


#include "cw/item.h"


#include "../modload.h"

int zyxel_register_actions_ac(struct cw_actiondef *def)
{

	int rc;
	rc = cw_actionlist_in_register_actions(def->in, actions_in);
	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

//	rc += cw_itemdefheap_register(def->items, _capwap_itemdefs);
//	rc += cw_itemdefheap_register(def->radioitems, cisco_radioitemdefs);

	intavltree_add(def->wbids, 0);



	return rc;
}



int zyxel_register_actions80211_ac(struct cw_actiondef *def)
{

	int rc;
	rc=0;
	rc = cw_actionlist_in_register_actions(def->in, actions80211_in);
	rc += cw_actionlist_out_register_actions(def->out, actions80211_out);
//	rc += cw_itemdefheap_register(def->items, cisco_itemdefs);
//	rc += cw_itemdefheap_register(def->radioitems, cisco_radioitemdefs);
	

/*	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);
*/


	return rc;
}


