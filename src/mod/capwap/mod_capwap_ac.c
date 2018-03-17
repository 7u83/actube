
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/ktv.h"
#include "cw/keys.h"


#include "mod_capwap.h"



extern int capwap_register_actions_ac(struct cw_actiondef *def);

static int init(struct cw_Mod * mod, mavl_t global_cfg, int role)
{
	switch (role){
		case CW_ROLE_AC:{
			cw_dbg(DBG_MOD, "CAPWAP: Initialiazing mod_capwap in AC mode");	
			cw_ktv_add_from_str(global_cfg,	
				"capwap/ac-descriptor/hardware/version",
				CW_TYPE_BSTR16,"0.0.0.1");
			cw_ktv_add_from_str(global_cfg,
				"capwap/ac-descriptor/hardware/vendor",CW_TYPE_DWORD,"0");
			cw_ktv_add_from_str(global_cfg,	
				"capwap/ac-descriptor/software/version",
				CW_TYPE_BSTR16,"0.0.0.1");
			cw_ktv_add_from_str(global_cfg,
				"capwap/ac-descriptor/software/vendor",CW_TYPE_DWORD,"0");
		}
		break;
	}
	

	return 0;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	if (mode != CW_MOD_MODE_CAPWAP)
		return 0;
	cw_dbg(DBG_MOD,"CAPWAP detected: yes");
	return 1;
}


static struct cw_Mod capwap_data = {
	"capwap",			/* name */
	init,				/* init */
	detect,				/* detect */
	capwap_register_msg_set,	/* register_messages */
	NULL,				/* dll_handle */
	NULL				/* data */
};



struct cw_Mod * mod_capwap(){
	return &capwap_data;
}
