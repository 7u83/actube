#ifndef __MOD_CIPWAP_H
#define __MOD_CIPWAP_H

struct cw_Mod * mod_cipwap_ac();
struct cw_Mod * mod_cipwap_wtp();

extern int cipwap_register_actions_ac(struct cw_actiondef *def);

#endif
