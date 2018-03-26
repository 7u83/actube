
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/cw.h"

#include "mod_capwap80211.h"





static int init()
{
	cw_dbg(DBG_INFO, "Initialiazing mod_capwap80211 ...");
	return 0;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	int wbid;
	if (mode != CW_MOD_MODE_BINDINGS)
		return 0;

	/* Check, if we can handle the Wireless Binding ID */
	wbid = cw_get_hdr_wbid(rawmsg);
	if (wbid != CAPWAP_WBID_IEEE80211 ) {
		cw_dbg(DBG_MOD,"CAPWAP80211 detected: no, my wbid=%d, remote wbid=%d",CAPWAP_WBID_IEEE80211,wbid);
		return 0;
	}
	cw_dbg(DBG_MOD,"CAPWAP80211 detected: yes");
	return 1;
}

/*
static int register_actions(struct cw_actiondef *actions, int mode)
{
	if (mode != CW_MOD_MODE_BINDINGS)
		return 0;

	return capwap80211_register_actions_ac(actions);
}
*/

struct cw_Mod mod_capwap80211 = {
	"capwap80211",			/* name */
	init,				/* init */
	detect,				/* detect */
	capwap80211_register_msgset,	/* register_messages */
	NULL,				/* dll_handle */
	NULL				/* data */
};

/*
static struct cw_Mod capwap80211_ac = {
	.name = "capwap80211",
	.init = init,
	.detect = detect,

};
*/

/*
struct cw_Mod *mod_capwap80211_ac()
{
	return &capwap80211_ac;
}
*/