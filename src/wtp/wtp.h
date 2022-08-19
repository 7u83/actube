#ifndef __WTP_H
#define __WTP_H

#include "cw/conn.h"
#include "cw/discovery.h"


extern int sulking_state();
//extern int join();
extern int discovery();
extern int image_update();
extern int setup_conf(struct cw_Conn * conn);
extern int configure();
extern int run();
extern int changestate();


struct cw_DiscoveryResults * cw_run_discovery(struct cw_Conn *conn, const char *addr, const char *bindaddr );

int join(struct cw_Conn * conn, struct cw_DiscoveryResults * dis);

struct cw_Conn * get_conn();


#define MAX_MODS 32
struct bootcfg{
	const char * modnames[MAX_MODS];
	int nmods;
	const char * modpath;
	const char * cfgfilename;
};

extern struct bootcfg bootcfg;


#endif
