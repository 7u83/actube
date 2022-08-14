#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <mavl.h>

#include "cfg.h"


struct cw_DiscoveryResults_elem {
	uint32_t key;
	char ip[64];
	cw_Cfg_t * cfg;
	int prio;
	int wtps;
	int ctr;
};

struct cw_DiscoveryResults
{
	int nr;
	struct mavl * list;
};

void cw_discovery_results_destroy(struct cw_DiscoveryResults *dis);
struct cw_DiscoveryResults * cw_discovery_results_create();
void cw_discovery_results_add(struct cw_DiscoveryResults *dis, cw_Cfg_t *ac_cfg, cw_Cfg_t * wtp_cfg);
void cw_discovery_results_destroy(struct cw_DiscoveryResults * r);

#endif
