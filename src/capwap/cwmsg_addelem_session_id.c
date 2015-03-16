#include "capwap.h"
#include "bstr.h"
#include "cwmsg.h"

void cwmsg_addelem_cwmsg_session_id(struct cwmsg *msg, bstr_t session_id)
{
	cwmsg_addelem(msg,CWMSGELEM_SESSION_ID,bstr_data(session_id),bstr_len(session_id));
}
