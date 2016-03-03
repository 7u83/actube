#include <stdint.h>

struct lwmsg{

	uint8_t * buffer;
	uint8_t * trnsprthdr;
	uint8_t * ctrlhdr;
	uint8_t * msgelems;
	int pos;
	int flags;
	int hlen;
	int rid;
	int seqnum;
	int type;

};

void lwmsg_init(struct lwmsg * lwmsg, uint8_t * buffer, uint8_t * macaddrss,int type, int seqnum );
