
#include "cw.h"
#include "dbg.h"
#include "log.h"
#include "stravltree.h"

/** 
 * @file 
 * @brief Implementation of LWAPP Vendor Specific Payload
 */

/**
 * Default handler for LWAPP Vendor Specific Payload message elements.
 */
int lw_in_vendor_specific(struct conn *conn, struct cw_action_in *a,
				  uint8_t * data, int len, struct sockaddr *from)
{

	cw_action_in_t as, *af;
	as = *a;

	as.vendor_id = cw_get_dword(data);
	as.elem_id = cw_get_word(data + 4);
	as.proto = CW_ACTION_PROTO_LWAPP;


	af = cw_actionlist_in_get(conn->actions->in, &as);

	if (!af) {
		cw_dbg(DBG_WARN,
		       "Can't handle Vendor Specific LWAPP Payload %s/%d, in msg %d (%s) in %s state.",
		       cw_strvendor(as.vendor_id), as.elem_id, as.msg_id,
		       cw_strmsg(as.msg_id), cw_strstate(as.capwap_state));
		return 0;
	}

	if (af->start) {
		int afrc = af->start(conn, af, data + 6, len - 6, from);
		if (af->mand && afrc) {
			/* add found mandatory message element 
			   to mand list */
			stravltree_add(conn->mand, af->item_id);
		}
		return afrc;
	}

	return 1;
}
