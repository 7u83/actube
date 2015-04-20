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


#ifndef __ACTION_H
#define __ACTION_H

#include <stdint.h>

#include "avltree.h"
#include "conn.h"
#include "mbag.h"
#include "strheap.h"
#include "intavltree.h"




/* Generic functions and structs */
void * cw_actionlist_add(struct avltree *t, void *a, size_t s);



/* Definitions for incomming messages */
struct cw_action_in{
	uint32_t vendor_id;
	uint8_t proto;
	uint8_t capwap_state;
	uint32_t msg_id;
	uint16_t elem_id;
	int (*start)(struct conn *conn,struct cw_action_in *a,uint8_t*data,int len,struct sockaddr *from);
	int (*end)(struct conn *conn,struct cw_action_in *a,uint8_t*elem,int len,struct sockaddr *from);
//	uint8_t itemtype;
	const struct mbag_typedef * itemtype;
	uint16_t item_id;
	uint16_t min_len;
	uint16_t max_len;
	uint8_t mand;
};

typedef int(*cw_action_fun_t)(struct conn *,struct cw_action_in *,uint8_t*,int ,struct sockaddr *);

typedef struct cw_action_in cw_action_in_t;

extern cw_action_fun_t cw_set_msg_end_callback(struct conn *conn, 
		int capwap_state,int msg_id, cw_action_fun_t callback);


typedef struct avltree * cw_actionlist_in_t;

extern cw_actionlist_in_t cw_actionlist_in_create();
extern cw_action_in_t * cw_actionlist_in_get(cw_actionlist_in_t t,cw_action_in_t *a);
extern cw_action_in_t * cw_actionlist_in_add(cw_actionlist_in_t t,cw_action_in_t *a);
extern int cw_actionlist_in_register_actions(cw_actionlist_in_t t,cw_action_in_t * actions);
extern cw_action_in_t *cw_actionlist_in_set_msg_end_callback(cw_actionlist_in_t a, 
				      uint8_t capwap_state,
					uint32_t msg_id,
				      int (*fun) (struct conn * conn,
						  struct cw_action_in * a, uint8_t * data,
						  int len,struct sockaddr *from));


cw_action_fun_t cw_set_msg_end_callback(struct conn *conn,
                int capwap_state,int msg_id, cw_action_fun_t callback);

/* Definitions for outgoing messages */
struct cw_action_out{
	uint32_t msg_id;
	uint32_t item_id;
	uint32_t vendor_id;
	uint16_t elem_id;
	int (*init)(struct conn * conn, struct cw_action_out *a, uint8_t * dst); 
	int (*out)(struct conn * conn, struct cw_action_out *a, uint8_t * dst); 
	struct mbag_item *(*get)(struct conn *conn,struct cw_action_out *a);
	uint8_t mand;
//	uint8_t itemtype;
	struct mbag_typedef * itemtype;
	
	void *defval;
	int xopt;

};

typedef struct cw_action_out cw_action_out_t;

typedef struct avltree *cw_actionlist_out_t;
extern cw_actionlist_out_t cw_actionlist_out_create();
extern cw_action_out_t * cw_actionlist_out_add(cw_actionlist_out_t t, struct cw_action_out * a);
extern int cw_actionlist_out_register_actions(cw_actionlist_out_t t,cw_action_out_t * actions);
#define cw_actionlist_out_get(t,a) avltree_get(t,a);

/**
 * Definition CAPWAP modes
 */
enum capwapmodes {
	/** Unknown -- means auto detect and 
	    set as soon as possible */
	CW_MODE_UNKNOWN,
	/** Standard mode as specified in RFC 5415 */
	CW_MODE_STD ,
	/** Cisco specific CAPWAP */
	CW_MODE_CISCO,
	/** CIPWAP, a mix of standard CAPWAP and some 
	Cisco extension */
	CW_MODE_CIPWAP,
	/** Zyxel */
	CW_MODE_ZYXEL
};




struct cw_actiondef{
	cw_actionlist_in_t in;
	cw_actionlist_out_t out;
	cw_strheap_t strmsg;
	cw_strheap_t strelem;

	/** Supported Wireless Binding IDs (WBID) */
	struct avltree * wbids;
};



#define cw_actionlist_get_node(t,a) avltree_get_node(t,a)

#endif


