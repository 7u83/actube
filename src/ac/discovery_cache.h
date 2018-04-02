#ifndef __DISCOVERY_CACHE_H
#define __DISCOVERY_CACHE_H


extern struct cw_DiscoveryCache;
struct cw_DiscoveryCacheElem{
	struct sockaddr_storage addr;
	const char *mod_capwap;
	const char *mod_bindings;
	uint32_t ctrlo, ctrhi;
};



struct cw_DiscoveryCache * discovery_cache_create(int len);
void discovery_cache_add(struct cw_DiscoveryCache *cache, 
	struct sockaddr * addr, const char * mod_capwap, const char * mod_bindings);
	
struct cw_DiscoveryCacheElem * discovery_cache_get(
		struct cw_DiscoveryCache * cache,struct sockaddr *addr);
		


#endif