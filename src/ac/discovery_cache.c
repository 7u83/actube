
#include "cw/mlist.h"
#include "cw/mavl.h"

struct cw_DiscoveryCache{
	int len;
	
};

struct CacheElem{
	struct sockaddr_storage addr;
	const char *mod_capwap;
	const char *mod_bindings;
	
};

void discovery_cache_destroy(struct cw_DiscoveryCache * c)
{
	
}

struct cw_DiscoveryCache * discovery_cache_init(int len){
	
	return NULL;
}