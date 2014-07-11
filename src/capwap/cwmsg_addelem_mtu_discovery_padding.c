#include <string.h>

#include "capwap.h"
#include "conn.h"
#include "cwmsg.h"

void cwmsg_addelem_mtu_discovery_padding(struct cwmsg * msg, struct conn* conn)
{
	int len = conn->mtu - (msg->msgelems-msg->buffer+msg->pos)-4;

	if (len < 0 )
		return;
	
	uint32_t val = CWMSGELEM_MTU_DISCOVERY_PADDING<<16|len;
	*((uint32_t*)(msg->msgelems+msg->pos))=htonl(val);
	memset(msg->msgelems+4+msg->pos,0xff,len);	
	msg->pos+=4+len;
}

