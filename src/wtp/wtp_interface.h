#ifndef __WTP_INTERFACE_H
#define __WTP_INTERFACE_H

#include <stdint.h>
#include "cw/bstr.h"

extern struct conn * get_conn();
extern bstr_t get_base_rmac();
uint8_t get_ecn_support();


#endif

