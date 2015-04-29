
#include "capwap.h"
#include "capwap_cisco.h"
#include "capwap_actions.h"
#include "cipwap.h"

/**
 *@file
 *brief CIPWAP Actions
 */

#define CW_ACTION_IN_CISCO_IMAGE_IDENTIFIER	 	\
	CW_ELEM_IMAGE_IDENTIFIER,			/* Element ID*/			\
	cw_in_cisco_image_identifier, 0,		/* start/end callback */	\
	MBAG_VENDORSTR, 				/* Type of element */		\
	CW_ITEM_IMAGE_IDENTIFIER,			/* ID to use store */		\
	1, 4096						/* min/max length */

#define CW_ACTION_IN_CIPWAP_WTP_DESCRIPTOR	\
	CW_ELEM_WTP_DESCRIPTOR,			/* Element ID */		\
	cw_in_cipwap_wtp_descriptor, 0,		/* start/end callback */	\
	0,									\
	CW_ITEM_WTP_DESCRIPTOR,							\
	8,1028

/* For CIPWAP we allow a 
   Session ID with 2 ... 16 bytes length */	
#define CW_ACTION_IN_CIPWAP_SESSION_ID	 	\
	CW_ELEM_SESSION_ID, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_BSTR, 		/* Type of element */		\
	CW_ITEM_SESSION_ID,		/* ID to use store */		\
	2, 16				/* min/max length */

#define CW_ACTION_IN_CIPWAP_AC_NAME	 	\
	CW_ELEM_AC_NAME, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_STR, 		/* Type of element */		\
	CW_ITEM_AC_NAME,		/* ID to use store */		\
	0, 512				/* min/max length */




cw_action_in_t cipwap_actions_ac_in[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Request IN
	 */

	{CW_VENDOR_ID_CISCO, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_CISCO_RAD_NAME, 
	cw_in_generic, 0, MBAG_STR,CW_ITEM_WTP_NAME,1,512}
	,

	{CW_VENDOR_ID_CISCO, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_CISCO_AP_GROUP_NAME, 
	cw_in_generic, 0, MBAG_STR,CW_ITEM_WTP_GROUP_NAME,1,512}
	,

	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST,
	 CW_ACTION_IN_CIPWAP_WTP_DESCRIPTOR,
	 1}
	,

	/* -------------------------------------------------------------------------------
	 * Join Request IN
	 */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, 0,
	 0, cw_in_check_cipwap_join_req}
	,

	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST,
	 CW_ACTION_IN_CIPWAP_WTP_DESCRIPTOR, 1}
	,
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST,
	 CW_ACTION_IN_CIPWAP_SESSION_ID, 1}
	,







	/* -------------------------------------------------------------------------------
	 * Image Data Request - Conig State
	 */

	{0, 0, CW_STATE_CONFIGURE, CW_MSG_IMAGE_DATA_REQUEST,
	 CW_ACTION_IN_CISCO_IMAGE_IDENTIFIER,
	 0}
	,

	
	/* Element: Result Code  
	   not mandatory in CIPWAP, while mandatory in CAPWAP
	 */
	{0, 0, CW_STATE_IMAGE_DATA, CW_MSG_IMAGE_DATA_RESPONSE,
	 CW_ACTION_IN_RESULT_CODE, 0}

	,

	/* ------------------------------------------------------------------------------- */

	/* Message: Configuration Status Request */

	/* Element: AC Name */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	 CW_ACTION_IN_CIPWAP_AC_NAME, 1}
	,


	{0,0,0}
};

extern struct cw_item *cw_get_local(struct conn *conn, struct cw_action_out *a);

cw_action_out_t cipwap_actions_ac_out[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Response OUT
	 */

	/* AC Descriptor (Cisco) */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0 ,
	CW_ELEM_AC_DESCRIPTOR, NULL,cw_out_cisco_ac_descriptor, 0}
	,

	/* Cisco AP Timesync - Important to get the WTP a DTLS 
	   connection established*/
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_TIMESTAMP, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_TIMESYNC, NULL,cw_out_cisco_ap_timesync, 0}
	,

	/* -------------------------------------------------------------------------------
	 * Join Response OUT
	 */

	/* AC Descriptor (Cisco) */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0 ,
	CW_ELEM_AC_DESCRIPTOR, NULL,cw_out_cisco_ac_descriptor, 0}
	,


	/* -------------------------------------------------------------------------------
	 * Echo Response OUT
	 */

	{CW_MSG_ECHO_RESPONSE, CW_ITEM_AC_TIMESTAMP, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_TIMESYNC, NULL,cw_out_cisco_ap_timesync, 0}
	,




	{0,0,0}
	
};

/**
 * Register CiPWAP actions
 */ 
int cw_register_actions_cipwap_ac(struct cw_actiondef *def)
{
	int rc;
	rc=cw_register_actions_capwap_ac(def);

	rc+=cw_actionlist_in_register_actions(def->in, cipwap_actions_ac_in);
	rc+=cw_actionlist_out_register_actions(def->out, cipwap_actions_ac_out);
	rc+=cw_strheap_register_strings(def->strelem,cipwap_strings_elem);
	return rc;
}

