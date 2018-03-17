
#include "cw/mod.h"

struct cw_Mod * modload_ac(const char *name);
struct cw_Mod * modload_wtp(const char *name);



#include "cipwap/mod_cipwap.h"
#include "capwap/mod_capwap.h"

#include "fortinet/mod_fortinet.h"
#include "zyxel/mod_zyxel.h"
#include "capwap80211/mod_capwap80211.h"

#define MODS_AC { mod_capwap_ac, mod_cipwap_ac,  /*mod_zyxel_ac,*/ mod_capwap80211_ac, mod_fortinet_ac, NULL }
#define MODS_WTP { mod_capwap_wtp, mod_cipwap_wtp,  mod_capwap80211_wtp, mod_fortinet_wtp, NULL }


