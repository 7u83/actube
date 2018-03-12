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

int cw_run_discovery(struct conn *conn, const char *acaddr, const char *bindaddr);


struct conn * get_conn();

#endif
