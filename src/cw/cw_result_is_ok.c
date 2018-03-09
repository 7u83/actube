#include "capwap.h"
#include "cw.h"

int cw_result_is_ok( int rc ) {
	if(rc == CAPWAP_RESULT_SUCCESS || rc == CAPWAP_RESULT_SUCCESS_NAT)
		return 1;
	return 0;
}
