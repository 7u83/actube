#ifndef __MOD_CISCO_H
#define __MOD_CISCO_H

struct cw_Mod * mod_cisco_ac();
struct cw_Mod * mod_cisco_wtp();

struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode);

#endif
