
#include <stdlib.h>
#include <string.h>

#include "wtpinfo.h"
#include "capwap.h"

#include "cw_util.h"
#include "cw_log.h"

int wtpinfo_readelem_session_id(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_SESSION_ID)
		return 0;

	wtpinfo->session_id = realloc(wtpinfo->session_id,len);
	memcpy(wtpinfo->session_id,msgelem,len);
	wtpinfo->session_id_len=len;
	return 1;
}


