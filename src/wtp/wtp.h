#ifndef __WTP_H
#define __WTP_H

#include "cw/conn.h"



extern int sulking_state();
extern int join();
extern int discovery();
extern int image_update();
extern int setup_conf(struct conn * conn);
extern int configure();
extern int run();
extern int changestate();



struct cw_DiscoveryResult{
	mavl_t prio_ip;
	mavl_t prio_ac;
	mlist_t results;
};

int cw_run_discovery(struct conn *conn, const char *acaddr, const char *bindaddr,
		struct cw_DiscoveryResult * dis);

int cw_discovery_init_results(struct cw_DiscoveryResult *dis);
void cw_discovery_free_results(struct cw_DiscoveryResult * dis);
int join(struct conn * conn, struct cw_DiscoveryResult * dis);

struct conn * get_conn();

#endif
