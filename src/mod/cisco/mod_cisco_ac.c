
#include "capwap/capwap.h"
#include "capwap/mod.h"
#include "capwap/log.h"
#include "capwap/dbg.h"

#include "capwap/action.h"

#include "mod_cisco.h"
#include "../modload.h"


static struct cw_actiondef actions;



extern int cisco_register_actions_ac(struct cw_actiondef *def);


static int init()
{
	cw_dbg(DBG_INFO,"Initialiazing mod_cisco ...");
	struct mod_ac * cmod = modload_ac("capwap");
	cmod->register_actions(&actions);
	if (!cmod ){
		cw_log(LOG_ERR,"Can't initzialize mod_cisco, failed to load bas mod mod_capwap");
		return 1;
	}

	int rc = cisco_register_actions_ac(&actions);
	cw_dbg(DBG_INFO,"Initialized mod cisco with %d actions",rc);
	return 0;
}


static int detect(struct conn *conn,const uint8_t *rawmsg, int rawlen,int elems_len,struct sockaddr *from)
{

	int offset = cw_get_hdr_msg_offset(rawmsg);
	const uint8_t *msg_ptr = rawmsg + offset;

	const uint8_t *elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	const uint8_t *elem;
	

	cw_foreach_elem(elem, elems_ptr, elems_len) {
		int id = cw_get_elem_id(elem);
		if (id==CW_ELEM_VENDOR_SPECIFIC_PAYLOAD){
			uint32_t vendor_id = cw_get_dword(cw_get_elem_data(elem));
			if (vendor_id==CW_VENDOR_ID_CISCO){
				conn->detected=1;
				conn->actions=&actions;
				return 1;

			}

		}

	}

	return 0;
}

static struct mod_ac capwap_ac = {
	.name ="cisco",
	.init = init,
	.detect = detect

};

struct mod_ac * mod_cisco_ac(){
	return &capwap_ac;
};

