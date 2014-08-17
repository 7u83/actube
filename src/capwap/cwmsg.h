#ifndef __CWMSG_H
#define __CWMSG_H

#include <stdint.h>

#include "radioinfo.h"
#include "acinfo.h"

struct cwmsg{
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

struct conn;

extern void cwmsg_init(struct cwmsg * cwmsg, uint8_t *buffer, int type, int seqnum, struct radioinfo * radioinfo);
//int cwmsg_send(struct cwmsg * cwmsg, int seqnum, int rid, struct conn * conn);
//
extern void cwmsg_addelem(struct cwmsg *msg,int type, const uint8_t *elem, int len);
extern void cwmsg_vaddelem(struct cwmsg *msg,int type,int n, ...);

extern void cwmsg_addelem_ac_descriptor(struct cwmsg *msg,struct ac_info * acinfo);
extern void cwmsg_addelem_ac_timestamp(struct cwmsg *msg);


extern void cwmsg_init_echo_request(struct cwmsg * cwmsg,uint8_t *buffer,struct conn * conn, struct radioinfo * radioinfo);
extern void cwmsg_addelem_mtu_discovery_padding(struct cwmsg * msg, struct conn* conn);
extern void cwmsg_addelem_image_identifier(struct cwmsg *msg,uint32_t vendor_id,uint8_t *img, int len);


#endif
