
#include "cw/cw.h"
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/capwap_items.h"

#include "cw/action.h"

#include "mod_cisco.h"
#include "../modload.h"

#include "cw/vendors.h"

#include "include/cipwap_items.h"

extern int cisco_register_actions80211_wtp(struct cw_actiondef *def);
extern int cisco_register_actions_wtp(struct cw_actiondef *def);

static int register_actions(struct cw_actiondef *actions, int mode)
{
	switch (mode) {
		case MOD_MODE_CAPWAP:
		{

			struct mod_wtp *cmod = modload_wtp("cipwap");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initzialize mod_cisco, failed to load base mod mod_cipwap");
				return 1;
			}
			
			cmod->register_actions(actions, MOD_MODE_CAPWAP);

			int rc = cisco_register_actions_wtp(actions);


			cw_dbg(DBG_INFO, "Initialized mod cisco with %d actions", rc);
			return rc;
		}
		case MOD_MODE_BINDINGS:
		{
			struct mod_ac *cmod = modload_wtp("capwap80211");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initzialize mod_cisco, failed to load base mod mod_capwap80211");
				return 1;
			}
			cmod->register_actions(actions, MOD_MODE_BINDINGS);
			int rc = cisco_register_actions80211_wtp(actions);
			cw_dbg(DBG_INFO, "Initialized mod cisco 80211 with %d actions", rc);
			return 0;
		}
		

	}


	return 0;


}

#include "cw/capwap_items.h"
mbag_t cisco_config_wtp;


static int init()
{
	cw_dbg(DBG_INFO, "Initialiazing mod_cisco ...");

	cisco_config_wtp = mbag_create();

	bstrv_t v;  
	v = bstrv_create_from_str(CW_VENDOR_ID_CISCO,".x01000000");
	mbag_set_bstr16(cisco_config_wtp,CW_ITEM_WTP_HARDWARE_VERSION,v);

	v = bstrv_create_from_str(CW_VENDOR_ID_CISCO,".x08007900");
	mbag_set_bstr16(cisco_config_wtp,CW_ITEM_WTP_SOFTWARE_VERSION,v);

	v = bstrv_create_from_str(CW_VENDOR_ID_CISCO,".x0C030800");
	mbag_set_bstr16(cisco_config_wtp,CW_ITEM_WTP_BOOTLOADER_VERSION,v);


//      struct mod_ac *cmod = modload_ac("capwap");
	return 1;
}

static int init_config(mbag_t config)
{
	bstr16_t gname = mbag_get_bstr16(config,CIPWAP_ITEM_WTP_GROUP_NAME,NULL);
	if (!gname){
		gname = bstr16_create_from_str("Entangled");
		mbag_set_bstr16(config,CIPWAP_ITEM_WTP_GROUP_NAME,gname);
	}


	mbag_t  board_data = mbag_get_mbag_c(config,CW_ITEM_WTP_BOARD_DATA,mbag_create);

	mbag_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR,CW_VENDOR_ID_CISCO);

//	mbag_set_bstr16(board_data, CW_ITEM_WTP_BOARD_MODELNO,bstr16_create_from_str("AIR-LAP1131AG-E-K9"));
	mbag_set_bstr16(board_data, CW_ITEM_WTP_BOARD_MODELNO,bstr16_create_from_str("AIR-LAP1142N-E-K9"));
	mbag_set_bstr16(board_data, CW_ITEM_WTP_BOARD_SERIALNO,bstr16_create_from_str("FCZ1441Q0XZ"));
	mbag_set_bstr16(board_data, CW_ITEM_WTP_BOARD_ID,bstr16_create_from_cfgstr(".x0000"));
	mbag_set_bstr16(board_data, CW_ITEM_WTP_BOARD_REVISION,bstr16_create_from_cfgstr(".x4230"));
//	"F8 66 F2 A3 42 FC"

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

static struct mod_ac cisco_wtp = {
	.name = "cisco",
	.init = init,
	.init_config = init_config,
	.detect = detect,
	.register_actions = register_actions
};

struct mod_ac *mod_cisco_wtp()
{
	return &cisco_wtp;
};
