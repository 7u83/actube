#ifndef __CWRMSG_H
#define __CWRMSG_H

struct cwrmsg{
	int rid;
	int wbid;
	uint8_t * msgelems;
	int msgelems_len;
	int type;
	int seqnum;
	int msglen;
#ifdef WITH_RMAC_SUPPORT
	uint8_t * rmac;
#endif

};

#endif
