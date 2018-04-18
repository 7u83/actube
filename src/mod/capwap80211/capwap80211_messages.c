
#include "cw/capwap.h"
#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/dot11.h"
#include "cw/capwap80211.h"


#include "cw/msgset.h"

#include "mod_capwap80211.h"

static int capwap80211_in_crate_set(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len);

static struct cw_ElemHandler handlers[] = {
	{
		"IEEE 802.11 WTP Radio Information",			/* name */
		CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		0, 0,						/* min/max length */
		CW_TYPE_DWORD,					/* type */
		"wtp-radio-information",			/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic				/* put */
	}
	,
	{
		"IEEE 802.11 Rate Set",				/* name */
		CAPWAP80211_ELEM_RATE_SET,			/* Element ID */
		0, 0,						/* Vendor / Proto */
		3, 0,						/* min/max length */
		CW_TYPE_BSTR16,					/* type */
		"rate_set",					/* Key */
		cw_in_radio_generic,					/* get */
		cw_out_radio_generic					/* put */
	}
	,
	{NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL}


};

static int discovery_request_states[] = { CAPWAP_STATE_DISCOVERY, 0 };
static struct cw_ElemDef discovery_request_elements[] = {
	{0, 0, CAPWAP80211_ELEM_WTP_RADIO_INFORMATION, 1, 0},
	{0, 0, 0, 0, 0}

};


static int join_request_states[] = { CAPWAP_STATE_JOIN, 0 };
static struct cw_ElemDef join_request_elements[] = {
	{0, 0, CAPWAP80211_ELEM_WTP_RADIO_INFORMATION, 1, 0},
	{0, 0, 0, 0, 0}

};

static int join_response_states[] = { CAPWAP_STATE_JOIN, 0 };
static struct cw_ElemDef join_response_elements[] = {
	{0, 0, CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,	1, 0},
	{0, 0, 0, 0, 0}

};

static int configuration_status_response_states[] = { CAPWAP_STATE_JOIN, 0 };
static struct cw_ElemDef configuration_status_response_elements[] = {
	{0, 0, CAPWAP80211_ELEM_RATE_SET		, 1, 0},
	{0, 0, 0, 0, 0}

};
static struct cw_MsgDef messages[] = {
	{
		NULL,
		CAPWAP_MSG_DISCOVERY_REQUEST,
		CW_ROLE_AC,
		discovery_request_states,
		discovery_request_elements
	}
	,
	{
		NULL,
		CAPWAP_MSG_JOIN_REQUEST,
		CW_ROLE_AC,
		join_request_states,
		join_request_elements
	}
	,
	{
		NULL,
		CAPWAP_MSG_JOIN_RESPONSE,
		CW_ROLE_AC,
		join_response_states,
		join_response_elements
	}
	,
	{
		NULL,
		CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,
		CW_ROLE_WTP,
		configuration_status_response_states,
		configuration_status_response_elements
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



static int capwap80211_in_crate_set(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
/*	int radio;
	int i;
	radio = cw_get_byte(data);
	for (i=0; i<len-1; i++){
		int rate = (data+1)[i];
		sprintf(d,"%0.1f",dot11_rate2float(val & 0x7f));
	}
*/	

}

