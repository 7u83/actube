#ifndef __MESSAGE_SET_H
#define __MESSAGE_SET_H

#include "cw.h"

extern void cw_message_set_destroy(cw_MsgSet_t * set);
extern cw_MsgSet_t * cw_message_set_create();
extern void cw_msgset_add(cw_MsgSet_t * set,
			cw_msgdef_t messages[]);
mlist_t cw_msgset_get_msg(cw_MsgSet_t * set, int type);
#endif