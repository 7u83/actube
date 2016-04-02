
#include "cw/cw.h"
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"

#include "mod_cisco.h"
#include "../modload.h"

#include "cw/vendors.h"


extern int cisco_register_actions80211_ac(struct cw_actiondef *def);
extern int cisco_register_actions_ac(struct cw_actiondef *def);

static int register_actions(struct cw_actiondef *actions, int mode)
{
	switch (mode) {
		case MOD_MODE_CAPWAP:
		{

			struct mod_ac *cmod = modload_ac("cipwap");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base mod mod_cipwap");
				return 1;
			}
			cmod->register_actions(actions, MOD_MODE_CAPWAP);
			int rc = cisco_register_actions_ac(actions);
			cw_dbg(DBG_INFO, "Initialized mod_cisco with %d actions", rc);
			return 0;
		}
		case MOD_MODE_BINDINGS:
		{
			struct mod_ac *cmod = modload_ac("capwap80211");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base mod mod_capwap80211");
				return 1;
			}
			cmod->register_actions(actions, MOD_MODE_BINDINGS);
			int rc = cisco_register_actions80211_ac(actions);
			cw_dbg(DBG_INFO, "Initialized mod_cisco 80211 with %d actions", rc);
			return 0;
		}
		

	}


	return 0;


}



static int init()
{
	cw_dbg(DBG_INFO, "Initialiazing mod_cisco ...");
//      struct mod_ac *cmod = modload_ac("capwap");
	return 1;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{


	int offset = cw_get_hdr_msg_offset(rawmsg);
	const uint8_t *msg_ptr = rawmsg + offset;

	const uint8_t *elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	const uint8_t *elem;


	/* To detect a Cisco AP we look for any vendor 
	 * specific payload Cisco identifier */
	cw_foreach_elem(elem, elems_ptr, elems_len) {
		int id = cw_get_elem_id(elem);
		if (id == CW_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
			uint32_t vendor_id = cw_get_dword(cw_get_elem_data(elem));
			if (vendor_id == CW_VENDOR_ID_CISCO) {
				//              conn->actions = &actions;
				if (mode == MOD_MODE_CAPWAP) {
					cw_dbg(DBG_MOD, "CISCO capwap detected: yes");
				} else {
					cw_dbg(DBG_MOD, "CISCO bindings detected: yes");
				}

				return 1;

			}

		}

	}

	if (mode == MOD_MODE_CAPWAP) {
		cw_dbg(DBG_MOD, "CISCO capwap detected: no");
	} else {
		cw_dbg(DBG_MOD, "CISCO bindings detected: no");
	}

	return 0;
}

static struct mod_ac capwap_ac = {
	.name = "cisco",
	.init = init,
	.detect = detect,
	.register_actions = register_actions
};

struct mod_ac *mod_cisco_ac()
{
	return &capwap_ac;
};
