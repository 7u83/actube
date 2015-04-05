
#include "conn.h"
#include "capwap.h"



void cw_init(struct conn * conn, uint8_t *buffer, int type, int seqnum, struct radioinfo * radioinfo)
{


}




void cw_init_response(struct conn * conn, uint8_t *req)
{
	uint8_t *buffer=conn->resp_buffer;
	int hbytes = cw_get_hdr_msg_offset(req);
	memcpy(buffer,req,hbytes);

	uint8_t * msgptr = req+hbytes;
	uint8_t * dmsgptr = buffer+hbytes;

	cw_set_msg_type(dmsgptr,cw_get_msg_type(msgptr)+1);
	cw_set_msg_seqnum(dmsgptr,cw_get_msg_seqnum(msgptr));
	cw_set_msg_flags(dmsgptr,0);


}
