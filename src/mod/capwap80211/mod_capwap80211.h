#ifndef __MOD_CAPWAP80211_H
#define __MOD_CAPWAP80211_H


#include "cw/msgset.h"

struct cw_MsgSet * capwap80211_register_msgset(struct cw_MsgSet * set, int mode);
/*
struct cw_Mod *mod_capwap80211_ac();
struct cw_Mod *mod_capwap80211_wtp();

extern int capwap80211_register_actions_ac(struct cw_actiondef *def);
extern int capwap80211_register_actions_wtp(struct cw_actiondef *def);
*/

#endif
