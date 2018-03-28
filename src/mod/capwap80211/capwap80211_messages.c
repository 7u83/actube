
#include "cw/capwap.h"
#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/capwap80211.h"


#include "cw/msgset.h"

#include "mod_capwap80211.h"



static struct cw_ElemHandler handlers[] = {
	{
		"80211 WTP Radio Information",			/* name */
		CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		0, 0,						/* min/max length */
		CW_TYPE_DWORD,					/* type */
		"wtp-radio-information",			/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic				/* put */
	}
	,
	{NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL}


};

static int discovery_request_states[] = { CAPWAP_STATE_DISCOVERY, 0 };

static struct cw_ElemDef discovery_request_elements[] = {
	{0, 0, CAPWAP80211_ELEM_WTP_RADIO_INFORMATION, 1, 0},
	{0, 0, 0, 0, 0}

};

static struct cw_MsgDef messages[] = {
	{
	 "Discovery Request",
	 CAPWAP_MSG_DISCOVERY_REQUEST,
	 CW_RECEIVER_AC,
	 discovery_request_states,
	 discovery_request_elements
	}
	,
	{0, 0, 0, 0, 0}
};

struct cw_MsgSet *capwap80211_register_msgset(set, mode)
struct cw_MsgSet *set;
int mode;
{
	if (mode != CW_MOD_MODE_BINDINGS)
		return NULL;

	cw_dbg(DBG_INFO, "CAPWAP80211: Register messages");
	cw_msgset_add(set, messages, handlers);
	cw_dbg(DBG_INFO, "CAPWAP0211: Done register messages");

	return set;
}
