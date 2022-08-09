#ifndef __ACTUBE_AC_H
#define __ACTUBE_AC_H

#include "mavl.h"
#include "cw/mavltypes.h"


extern struct ac_status ac_global_status;


int actube_global_init();


/* the protocol we understand */

enum {
	AC_PROTO_CAPWAP=0,
	AC_PROTO_LWAPP=1,
	AC_PROTO_UNKNOWN
};



#endif

