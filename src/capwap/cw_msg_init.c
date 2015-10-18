
#include "conn.h"
#include "capwap.h"



void cw_init(struct conn * conn, uint8_t *buffer, int type, int seqnum, struct radioinfo * radioinfo)
{


}



void cw_init_response(struct conn * conn, uint8_t *req)
{
	uint8_t *buffer=conn->resp_buffer;
	int shbytes = cw_get_hdr_msg_offset(req);
	int dhbytes;
	memcpy(buffer,req,shbytes);
	cw_set_hdr_hlen(buffer,2);
	cw_set_hdr_flags(buffer,CW_FLAG_HDR_M,1);
	dhbytes = cw_get_hdr_msg_offset(buffer);

	uint8_t * msgptr = req+shbytes;
	uint8_t * dmsgptr = buffer+dhbytes;

	cw_set_msg_type(dmsgptr,cw_get_msg_type(msgptr)+1);
	cw_set_msg_seqnum(dmsgptr,cw_get_msg_seqnum(msgptr));
	cw_set_msg_flags(dmsgptr,0);
}
