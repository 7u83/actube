#include "cw/capwap.h"
#include "cw/mbag.h"

extern struct ac_status ac_global_status;
extern int ac_global_init();
extern mbag_t ac_config;	
extern struct cw_actiondef capwap_actions;


/* the protocol we understand */

enum {
	AC_PROTO_CAPWAP=0,
	AC_PROTO_LWAPP=1,
	AC_PROTO_UNKNOWN
};



