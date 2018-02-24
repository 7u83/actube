#ifndef __MESSAGE_SET_H
#define __MESSAGE_SET_H

#include "cw.h"

extern void cw_message_set_destroy(cw_message_set_t * set);
extern cw_message_set_t * cw_message_set_create();
extern void cw_message_set_add(cw_message_set_t * set,
			cw_msgdef_t messages[]);
mlist_t cw_msgset_get_msg(cw_message_set_t * set, int type);
#endif