#ifndef __WTP_H
#define __WTP_H

#include "capwap/conn.h"



extern int sulking_state();
extern int join();
extern int discovery();
extern int image_update();


struct conn * get_conn();

#endif
