#ifndef __MOD_CAPWAP80211_H
#define __MOD_CAPWAP80211_H

struct mod_ac *mod_capwap80211_ac();
struct mod_wtp *mod_capwap80211_wtp();

extern int capwap80211_register_actions_ac(struct cw_actiondef *def);
extern int capwap80211_register_actions_wtp(struct cw_actiondef *def);


#endif
