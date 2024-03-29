
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/val.h"
#include "cw/keys.h"
#include "cw/dtls.h"
#include "cw/cfg.h"

#include "mod_capwap.h"

static int init(struct cw_Mod * mod, mavl_t global_cfg, int role)
{
	cw_dbg(DBG_INFO,"CAPWAP: Inititalizing mod_capwap.");
	
	switch (role){
		case CW_ROLE_AC:{
			cw_dbg(DBG_MOD, "CAPWAP: Initialiazing mod_capwap in AC mode");	
			cw_cfg_set(global_cfg,	
				"capwap/ac-descriptor/hardware/version",
				"0.0.0.1");
			cw_cfg_set(global_cfg,
				"capwap/ac-descriptor/hardware/vendor","0");
			cw_cfg_set(global_cfg,	
				"capwap/ac-descriptor/software/version","0.0.0.1");
			cw_cfg_set(global_cfg,
				"capwap/ac-descriptor/software/vendor", "0");
		}
		break;
	}
	

	return 0;
}


static int detect(struct cw_Conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	if (mode != CW_MOD_MODE_CAPWAP)
		return 0;
	cw_dbg(DBG_MOD,"CAPWAP detected: yes");
	return 1;
}


int static setup_cfg(struct cw_Conn  * conn)
{
	int security;
	security = cw_setup_dtls(conn,conn->local_cfg,"capwap",CAPWAP_CIPHER);


//	stop();
//	cw_ktv_set_byte(conn->local_cfg,"ac-descriptor/security",security);

	return 0;
}


struct cw_Mod mod_capwap = {
	"capwap",			/* name */
	init,				/* init */
	detect,				/* detect */
	capwap_register_msg_set,	/* register_messages */
	NULL,				/* dll_handle */
	NULL,				/* data */
	setup_cfg			/* setup_cfg */
};
