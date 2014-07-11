
#include "cwmsg.h"

void cwmsg_set_control_header(struct cwmsg * cwmsg,int msgtype, int seqnum)
{
	uint32_t val;
	val = htonl(msgtype);
	*((uint32_t*)(cwmsg->ctrlhdr))=htonl(val);
	val = (seqnum<<24)|(cwmsg->pos<<8);
	*((uint32_t*)(cwmsg->ctrlhdr+4))=htonl(val);
}


