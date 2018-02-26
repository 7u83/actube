#include <confuse.h>

#include "cw/cw.h"
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"

#include "mod_cisco.h"
#include "../modload.h"

#include "cw/vendors.h"

#include "cw/capwap_items.h"


extern int cisco_register_actions80211_ac(struct cw_actiondef *def);
extern int cisco_register_actions_ac(struct cw_actiondef *def);

mbag_t cisco_config = NULL;


static struct cw_MsgSet * register_messages(struct cw_MsgSet *set, int mode)
{
	switch (mode) {
		case CW_MOD_MODE_CAPWAP:
		{

			struct cw_Mod *cmod = cw_mod_load("capwap");//  NULL; //modload_ac("cipwap");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base module mod_cipwap");
				return 1;
			}
			cmod->register_messages(set, CW_MOD_MODE_CAPWAP);
			cisco_register_msg_set(set,CW_MOD_MODE_CAPWAP);
			cw_dbg(DBG_INFO, "Initialized mod_cisco with %d messafe", 7);
			return 0;
		}
		case MOD_MODE_BINDINGS:
		{
			struct cw_Mod *cmod = NULL; //modload_ac("capwap80211");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base mod mod_capwap80211");
				return 1;
			}
			cmod->register_messages(set, MOD_MODE_BINDINGS);
			int rc = cisco_register_actions80211_ac(set);
			cw_dbg(DBG_INFO, "Initialized mod_cisco 80211 with %d actions", 12);
			return 0;
		}
		

	}


	return 0;


}

static void errfunc(cfg_t *cfg, const char *fmt, va_list ap){
	
	if (cfg && cfg->filename && cfg->line)
		cw_log(LOG_ERR, "MOD Cisco cfg file in %s:%d: ", 
			cfg->filename, cfg->line);
	else if (cfg && cfg->filename)
		cw_log(LOG_ERR, "MOD Cisco cfg file in %s:", cfg->filename);
}

static int init()
{
	int rc = 1;
	cw_dbg(DBG_INFO, "Initialiazing mod_cisco ...");
	cisco_config = mbag_create();

	char * hardware_version = strdup(".x01000001");
	char * software_version = NULL;

	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("hardware_version", &hardware_version),
		CFG_SIMPLE_STR("software_version",&software_version),
		CFG_END()
	};

	cfg_t *cfg;
	cfg = cfg_init(opts, CFGF_NONE);
	
	cfg_set_error_function(cfg, errfunc);
	
	char *filename = "cisco.conf";
	FILE * f = fopen(filename,"r");
	if (f){
		fclose(f);
		if (cfg_parse(cfg, filename)){
			rc = 0;
			goto errX;
		}
	}

	uint8_t * str;
	
	str = bstr_create_from_cfgstr(hardware_version);
	mbag_set_bstrv(cisco_config, CW_ITEM_AC_HARDWARE_VERSION, 
		CW_VENDOR_ID_CISCO, 
		bstr_data(str),bstr_len(str)
		);
	free(str);

	if (software_version){
		str = bstr_create_from_cfgstr(software_version);
		mbag_set_bstrv(cisco_config, CW_ITEM_AC_SOFTWARE_VERSION, 
			CW_VENDOR_ID_CISCO, 
			bstr_data(str),bstr_len(str)
			);
		free(str);
	}
errX:
	if (hardware_version)
		free (hardware_version);
	if (software_version)
		free(software_version);
	return rc;
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
		if (id == CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
			uint32_t vendor_id = cw_get_dword(cw_get_elem_data(elem));
			if (vendor_id == CW_VENDOR_ID_CISCO) {
				//              conn->actions = &actions;
				if (mode == CW_MOD_MODE_CAPWAP) {
					cw_dbg(DBG_MOD, "CISCO capwap detected: yes");
				} else {
					cw_dbg(DBG_MOD, "CISCO bindings detected: yes");
				}

				return 1;

			}

		}

	}

	if (mode == CW_MOD_MODE_CAPWAP) {
		cw_dbg(DBG_MOD, "CISCO capwap detected: no");
	} else {
		cw_dbg(DBG_MOD, "CISCO bindings detected: no");
	}

	return 0;
}

static struct cw_Mod capwap_ac = {
	.name = "cisco",
	.init = init,
	.detect = detect,
//	.register_actions = register_actions,
	.register_messages = register_messages
};

struct cw_Mod *mod_cisco_ac()
{
	return &capwap_ac;
};


struct cw_Mod *mod_cisco()
{
	return &capwap_ac;
};
