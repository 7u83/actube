#ifndef __DISCOVERY_CACHE_H
#define __DISCOVERY_CACHE_H



#include "cw/mod.h"


struct cw_DiscoveryCache * discovery_cache_create(int len);

void discovery_cache_add(struct cw_DiscoveryCache *cache, 
	struct sockaddr * addr, struct cw_Mod  * mod_capwap, struct cw_Mod * mod_bindings);

int discovery_cache_get(struct cw_DiscoveryCache * cache,struct sockaddr *addr, 
			struct cw_Mod ** modcapwap, struct cw_Mod **modbindings);

void discovery_cache_destroy(struct cw_DiscoveryCache * c);
#endif
