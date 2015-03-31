
#ifndef __MSGTREE_H
#define __MSGTREE_H


#include <stdint.h>

#include "avltree.h"
#include "conn.h"

struct cw_action{
	uint32_t vendor_id;
	uint8_t proto;
	uint8_t capwap_state;
	uint32_t msg_id;
	uint16_t elem_id;

	int (*start)(struct conn *conn,struct cw_action *a,uint8_t*data,int len);
	int (*end)(struct conn *conn,struct cw_action *a,uint8_t*elem,int len);

	uint16_t min_len;
	uint16_t max_len;

	uint8_t capwap_state_next;
	
	
	const char *name;
};
typedef struct cw_action cw_action_t;
 

typedef struct avltree * cw_actionlist_t;


extern cw_actionlist_t cw_actionlist_create();
extern cw_action_t * cw_actionlist_get(cw_actionlist_t t,cw_action_t *a);
extern cw_action_t * cw_actionlist_add(cw_actionlist_t t,cw_action_t *a);
extern int cw_register_actions(cw_actionlist_t t,cw_action_t * actions);

int check(uint8_t *elem,int len);


#endif


