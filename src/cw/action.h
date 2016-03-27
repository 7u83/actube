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

/**
 * @file
 * @brief CAPWAP ACtions Header
 * @defgroup ACTION CAPWAP/LWAPP Actions
 * @{
 */ 
#ifndef __ACTION_H
#define __ACTION_H

#include <stdint.h>

#include "mavl.h"
#include "conn.h"
#include "mbag.h"
#include "strheap.h"
#include "intavltree.h"
#include "item.h"
#include "mlist.h"


struct conn;


/**
 * @file action.h
 * @brief Header for actions
 */


/**
 * @defgroup ACTION Action
 * @{
 */

/**
 * Definition of an action for incomming messages 
 */
struct cw_action_in{
	uint32_t vendor_id;
	uint8_t proto;
	uint8_t capwap_state;
	uint32_t msg_id;
	uint16_t elem_id;
	int (*start)(struct conn *conn,struct cw_action_in *a,uint8_t*data,int len,struct sockaddr *from);
	int (*end)(struct conn *conn,struct cw_action_in *a,uint8_t*elem,int len,struct sockaddr *from);
	const struct mbag_typedef * itemtype;
	const char * item_id;
	uint16_t min_len;
	uint16_t max_len;
	uint8_t mand;
};


/** a handy type for incomming actions */
typedef struct cw_action_in cw_action_in_t;

/** Definition of an action list for incomming messages */
typedef mavl_t cw_actionlist_in_t;


extern cw_actionlist_in_t cw_actionlist_in_create();
extern cw_action_in_t * cw_actionlist_in_get(cw_actionlist_in_t t,cw_action_in_t *a);
//extern cw_action_in_t * cw_actionlist_in_add(cw_actionlist_in_t t,cw_action_in_t *a);
extern int cw_actionlist_in_register_actions(cw_actionlist_in_t t,cw_action_in_t * actions);



/**
 * Definitioni of an action  foroutgoing messages 
 * */
struct cw_action_out{
	uint32_t msg_id;
	const char * item_id;
	uint32_t vendor_id;
	uint16_t elem_id;

	int (*init)(struct conn * conn, struct cw_action_out *a, uint8_t * dst); 
	int (*out)(struct conn * conn, struct cw_action_out *a, uint8_t * dst); 
	struct mbag_item *(*get)(struct conn *conn,struct cw_action_out *a);
	uint8_t mand;
	struct mbag_typedef * itemtype;
	void *defval;
};


typedef struct cw_action_out cw_action_out_t;
typedef struct mavl *cw_actionlist_out_t;


extern cw_actionlist_out_t cw_actionlist_out_create();
extern int cw_actionlist_out_register_actions(cw_actionlist_out_t t,cw_action_out_t * actions);
mlist_t * cw_actionlist_out_get(cw_actionlist_out_t,int msg_id);

//extern cw_action_out_t * cw_actionlist_out_add(cw_actionlist_out_t t, struct cw_action_out * a);

/**
 * @}
 */









extern cw_actionlist_out_t cw_actionlist_out_create();
extern cw_action_out_t * cw_actionlist_out_add(cw_actionlist_out_t t, struct cw_action_out * a);
extern int cw_actionlist_out_register_actions(cw_actionlist_out_t t,cw_action_out_t * actions);

/**
 * Definition CAPWAP modes
 */
enum capwapmodes {
	/** Auto -- means auto detect the CAPWAP mode and 
	    it set as soon as possible */
	CW_MODE_AUTO,
	/** Standard CAPWAP mode as specified in RFC 5415 */
	CW_MODE_CAPWAP,
	/** Cisco specific CAPWAP */
	CW_MODE_CISCO,
	/** CIPWAP, a mix of standard CAPWAP and some 
	Cisco extension */
	CW_MODE_CIPWAP,
	/** Zyxel */
	CW_MODE_ZYXEL,
	/** Lancom */
	CW_MODE_LANCOM,
	/** Fortinet */
	CW_MODE_FORTINET
};






struct cw_actiondef{
	cw_actionlist_in_t in;
	cw_actionlist_out_t out;
	cw_strheap_t strmsg;
	cw_strheap_t strelem;
	cw_itemdefheap_t items;
	cw_itemdefheap_t radioitems;

	mlist_t * mout; 

	/** Supported Wireless Binding IDs (WBID) */
	struct avltree * wbids;
};



extern struct outelem * cw_actionlist_out_get_mlist(cw_actionlist_out_t t, int msg_id);


typedef int(*cw_action_fun_t)(struct conn *,struct cw_action_in *,uint8_t*,int,struct sockaddr *);


cw_action_fun_t cw_set_msg_end_callback(struct cw_actiondef * actions, 
		int capwap_state,int msg_id, cw_action_fun_t callback);


#define cw_actionlist_get_node(t,a) avltree_get_node(t,a)

/**@}*/

#endif


