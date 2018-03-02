#include "capwap.h"


const char * cw_strerror(int rc) {
	if (rc<0){
		if (errno==EAGAIN)
			return "Timed out";
		return strerror(errno);
	}
	return cw_strresult(rc);
}



int cw_rcok(int rc)
{
	if (rc==0 || rc==2){
		return 1;
	}
	return 0;
}


/**
 *@defgroup CW CW 
 *@{ 
 */

const char *cw_strelemp_(cw_strheap_t h, int msg_id)
{
	const char *rc = cw_strheap_get(h, msg_id);
	if (rc)
		return rc;
	return cw_strheap_get(h, CW_STR_STOP);
}

/**@}*/


