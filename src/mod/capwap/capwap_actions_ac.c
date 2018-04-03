/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "cw/cw.h"
#include "cw/ktv.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/msgset.h"
#include "cw/keys.h"

#include "mod_capwap.h"


static struct cw_ElemHandler handlers[] = {

	{ 
		"Discovery Type",			/* name */
		CAPWAP_ELEM_DISCOVERY_TYPE,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_DISCOVERY_TYPE,			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"WTP Mac Type",				/* name */
		CAPWAP_ELEM_WTP_MAC_TYPE,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_WTP_MAC_TYPE,			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"WTP Board Data",			/* name */
		CAPWAP_ELEM_WTP_BOARD_DATA,		/* Element ID */
		0,0,					/* Vendor / Proto */
		14,1024,				/* min/max length */
		NULL,					/* type */
		CW_KEY_WTP_BOARD_DATA,			/* Key */
		capwap_in_wtp_board_data,		/* get */
		capwap_out_wtp_board_data		/* put */
	}
	,
	{ 
		"WTP Descriptor",			/* name */
		CAPWAP_ELEM_WTP_DESCRIPTOR,		/* Element ID */
		0,0,					/* Vendor / Proto */
		33,1024,				/* min/max length */
		NULL,					/* type */
		CW_KEY_WTP_DESCRIPTOR,			/* Key */
		capwap_in_wtp_descriptor,		/* get */
		capwap_out_wtp_descriptor,		/* put */
	}
	,
	{ 
		"WTP Frame Tunnel Mode",		/* name */
		CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,	/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_WTP_FRAME_TUNNEL_MODE,		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"Vendor Specific Payload",		/* name */
		CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		"vendor_specific_payload",		/* Key */
		capwap_in_vendor_specific_payload,	/* get */
		NULL					/* put */
	}
	,
	{ 
		"MTU Discovery Padding",		/* name */
		CAPWAP_ELEM_MTU_DISCOVERY_PADDING,	/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		"mtu_discovery_padding",		/* Key */
		capwap_in_mtu_discovery_padding,	/* get */
		NULL					/* put */
	}
	,

	{ 
		"AC Descriptor",			/* name */
		CAPWAP_ELEM_AC_DESCRIPTOR,		/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		CW_KEY_AC_DESCRIPTOR,			/* Key */
		capwap_in_ac_descriptor,		/* get */
		capwap_out_ac_descriptor		/* put */
	}
	,

	{ 
		"AC Name",				/* name */
		CAPWAP_ELEM_AC_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"ac-name",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"CAPWAP Control IPv4 Address",		/* name */
		CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		6,6,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap-control-ip-address",		/* Key */
		capwap_in_capwap_control_ip_address,	/* get */
		capwap_out_capwap_control_ip_address	/* put */
	},
	
	{ 
		"CAPWAP Control IPv6 Address",		/* name */
		CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		18,18,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap-control-ip-address",		/* Key */
		capwap_in_capwap_control_ip_address,	/* get */
		capwap_out_capwap_control_ip_address	/* put */
	}
	
	,

	{ 
		"Location Data",			/* name */
		CAPWAP_ELEM_LOCATION_DATA,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,CAPWAP_MAX_LOCATION_DATA_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"location-data",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"WTP Name",				/* name */
		CAPWAP_ELEM_WTP_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		1,CAPWAP_MAX_WTP_NAME_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"wtp-name",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"Session ID",				/* name */
		CAPWAP_ELEM_SESSION_ID,			/* Element ID */
		0,0,					/* Vendor / Proto */
		CAPWAP_SESSION_ID_LEN,CAPWAP_SESSION_ID_LEN,	/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"session-id",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,



	{0,0,0,0,0,0,0,0}

};


static int discovery_request_states[] = {CAPWAP_STATE_DISCOVERY,0};
static struct cw_ElemDef discovery_request_elements[] ={
	{0,0,CAPWAP_ELEM_DISCOVERY_TYPE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_BOARD_DATA,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_DESCRIPTOR,		1, 0},	
	{0,0,CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_MAC_TYPE,			1, 0},
	{0,0,CAPWAP_ELEM_MTU_DISCOVERY_PADDING,		0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};


static int discovery_response_states[] = {CAPWAP_STATE_DISCOVERY,0};
static struct cw_ElemDef discovery_response_elements[] ={
	{0,0,CAPWAP_ELEM_AC_DESCRIPTOR,			1, 0},
	{0,0,CAPWAP_ELEM_AC_NAME,			1, 0},
	{0,0,CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,	1, 0},
	{0,0,CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}

};

static int join_request_states[] = {CAPWAP_STATE_JOIN,0};
static struct cw_ElemDef join_request_elements[] ={
	{0,0,CAPWAP_ELEM_LOCATION_DATA,			1, 0},
	{0,0,CAPWAP_ELEM_WTP_BOARD_DATA,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_DESCRIPTOR,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_NAME,			1, 0},
	{0,0,CAPWAP_ELEM_SESSION_ID,			1, 0},
	{0,0,CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_MAC_TYPE,			1, 0},
	
/*	{0,0,CAPWAP_ELEM_DISCOVERY_TYPE,		1, 0},

	{0,0,CAPWAP_ELEM_WTP_DESCRIPTOR,		1, 0},	
	{0,0,CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_MAC_TYPE,			1, 0},
	{0,0,CAPWAP_ELEM_MTU_DISCOVERY_PADDING,		0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
*/
	{0,0,0,0,0}
};



static struct cw_MsgDef messages[] = {
	{
		"Discovery Request",
		CAPWAP_MSG_DISCOVERY_REQUEST,
		CW_ROLE_AC,
		discovery_request_states,
		discovery_request_elements
	},
	
	{
		"Discovery Response",
		CAPWAP_MSG_DISCOVERY_RESPONSE,
		CW_ROLE_WTP,
		discovery_response_states,
		discovery_response_elements
	},

	{
		"Join Request",
		CAPWAP_MSG_JOIN_REQUEST,
		CW_ROLE_AC,
		join_request_states,
		join_request_elements
	},

	
	
/*	{ 
		"Discovery Request",
		CAPWAP_MSG_DISCOVERY_REQUEST,
		CW_RECEIVER_AC,
		(int[]){CAPWAP_STATE_DISCOVERY,0}
		
	},
*/

	/* Discovery Request Message*/
/*	{
		.name = "Discovery Request",
		.type = CAPWAP_MSG_DISCOVERY_REQUEST,
		.receiver = CW_RECEIVER_AC,
		.states = (int[]){CAPWAP_STATE_DISCOVERY,0},
		.elements = (cw_ElemDef_t []){
			{&_DISCOVERY_TYPE,1},
			{&_WTP_BOARD_DATA,1},
			{&_WTP_DESCRIPTOR,1},
			{&_WTP_FRAME_TUNNEL_MODE,1},
			{&_WTP_MAC_TYPE,1},
			{&_MTU_DISCOVERY_PADDING,0},
			{&_VENDOR_SPECIFIC_PAYLOAD,0},
			{0,0},
		}
	},
*/
	/* Discovery Request Response */
/*	{
		.name = "Discovery Response",
		.type = CAPWAP_MSG_DISCOVERY_RESPONSE,
		.receiver = CW_RECEIVER_WTP,
		.states = (int[]){CAPWAP_STATE_DISCOVERY,0},
		.elements = (cw_ElemDef_t[]){
			{&_AC_DESCRIPTOR,1},
			{0,0},
		}
	},
*/	{0,0,0,0}
	
	
	
};


struct cw_MsgSet * capwap_register_msg_set(struct cw_MsgSet * set, int mode){
	if (mode != CW_MOD_MODE_CAPWAP)
		return NULL;
	
	cw_dbg(DBG_INFO,"CAPWAP: Register messages");
	
	cw_msgset_add(set,messages, handlers);
	mavl_add_ptr(set->types_tree,CW_TYPE_BSTR16);
	mavl_add_ptr(set->types_tree,CW_TYPE_BYTE);
	mavl_add_ptr(set->types_tree,CW_TYPE_DWORD);
	mavl_add_ptr(set->types_tree,CW_TYPE_WORD);

	cw_dbg(DBG_INFO,"CAPWAP: Done register messages");
	return set;
}

void test_sets(){

	struct cw_MsgSet * set = cw_msgset_create();
	if (set==NULL){
		cw_log(LOG_ERR,"No mem");
		return;
	}
	
	cw_msgset_add(set,messages, handlers);

	
}
/*
	cw_msgset_add(set,messages);
	
	cw_elem_handler_t el; 
	memset(&el,0,sizeof(el));
	el.id=CAPWAP_ELEM_DISCOVERY_TYPE;
	
	cw_ElemDef_t search, *result;;
	search.handler = &el;
	
	
	result = mavl_find(set->all_elems,&search);
	
	if (result!=NULL){
		printf("Found: %d %s %d\n",
			result->handler->id,
			result->handler->name,
			result->mand
		);
	}
	else{
		printf("not found\n");
	}

	mlist_t m;
	m = cw_msgset_get_msg(set,CAPWAP_MSG_DISCOVERY_REQUEST);
	printf("Hey: %p\n",m);	
	
	struct mlist_elem * i;
	mlist_foreach(i,m){
		cw_ElemDef_t * le = i->data;
		printf("Element: %d %s\n",le->handler->id, le->handler->name, le->mand);
	}
	



}






*/





