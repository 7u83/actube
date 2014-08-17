
#include "capwap.h"

void cwmsg_addelem_maximum_message_length(struct cwmsg *msg,int len)
{
	uint8_t c[2];
       	*((uint16_t*)c)= htons(len);
	cwmsg_addelem(msg,CWMSGELEM_MAXIMUM_MESSAGE_LENGTH,c,4);
}

