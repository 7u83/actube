#ifndef __MOD_CIPWAP_H
#define __MOD_CIPWAP_H

struct mod_ac * mod_cipwap_ac();
struct mod_wtp * mod_cipwap_wtp();

extern int cipwap_register_actions_ac(struct cw_actiondef *def);

#endif
