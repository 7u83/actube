#include <stdlib.h>

#include "capwap.h"
#include "cw_log.h"
#include "conn.h"


static int cwrmsg_init_ctrlhdr(struct cwrmsg * cwrmsg, uint8_t * msg, int len)
{
	if (len<8)
		return 0;

	uint32_t val;
	val = ntohl(*((uint32_t*)(msg+0)));


	cwrmsg->type = ntohl(*((uint32_t*)(msg))); 


	val = ntohl(*((uint32_t*)(msg+4)));
	cwrmsg->seqnum = CW_GET_DWORD_BITS(val,0,8);
	cwrmsg->msgelems_len=CW_GET_DWORD_BITS(val,8,16)-3;
//	ch->flags=CW_GET_DWORD_BITS(val,24,8);
	cwrmsg->msgelems=msg+8;

	if (8+cwrmsg->msgelems_len != len){
		return 0;
	}

	return 1;

}


static int process_message(struct conn * conn,struct cwrmsg *cwrmsg,int (*cb)(void*,struct cwrmsg *),void *cbarg)
{
	if (!(cwrmsg->type & 0x1)) {
		/* It's a response  message, no further examination required. */
//		conn->process_message(conn->pmsgarg,cwrmsg);
		cb(cbarg,cwrmsg);
		return 0;
	}

	/* It's a request message, check if seqnum is right and if
	 * we have already sent a response message*/

	int s1=conn->last_seqnum_received;
	int s2=cwrmsg->seqnum;
	int sd=s2-s1;

	if ((sd>0 && sd<128) || (sd<0 && sd<-128) || s1<0){
		/* seqnum is ok, normal message processing */
//		conn->last_seqnum_received=cwrmsg->seqnum;
		cb(cbarg,cwrmsg);
		return 0;
	}

	if (sd != 0)
	{
		cw_log_debug0("Discarding message, old seqnum, seqnum = %d, last seqnum=%d",s2,s1);
		return 1;
	}

	/* the received request message was retransmitte by our peer,
	 * let's retransmit our response message if we have one*/

	cw_log_debug0("Retransmitted request message detected, seqnum=%d",s2);

	if (!conn->last_response){
		cw_log_debug0("No cached response for retransmission, request seqnum=%d",s2);
		return 0;
	}

	cw_log_debug0("Retransmitting response message, seqnum=%d",s2);
//	cwmsg_send(conn->last_response,conn->last_response_seqnum,conn->last_response_rid,conn);
	conn_send_cwmsg(conn,conn->last_response);
	return 1;	
}



void conn_process_packet(struct conn * conn, uint8_t *packet, int len,int (*cb)(void*,struct cwrmsg*),void *cbarg)
{

	cw_log_debug2("Process conn Packet received, len=%d",len);

	if (len<8){
		/* packet too short */
		cw_log_debug1("Discarding packet, packet too short, len=%d",len);
		return;
	}

	uint32_t val = ntohl(*((uint32_t*)packet));

	int preamble = val >> 24;
	if ( (preamble & 0xf0) != CW_VERSION){
		/* wrong version */
		cw_log_debug1("Discarding packet, wrong version, version=%d",preamble&0xf0);
		return;
	}



	if (preamble & 0xf ) {
		/* decode dtls */
		return;
	}

	int hlen = 4*((val >> 19) & 0x1f);


	int payloadlen = len - hlen;
	if (payloadlen<0){
		cw_log_debug1("Discarding packet, hlen greater than len, hlen=%d",hlen);
		/* EINVAL */
		return;
	}


	struct cwrmsg cwrmsg;
	cwrmsg.wbid=(val>>9) & 0x1f;
	cwrmsg.rid=(val>>14) & 0x1f;

#ifdef WITH_RMAC_SUPPORT
	if (val & CWTH_FLAGS_M){
		if (*(packet+8)+8>hlen){
			cw_log_debug0("Discarding packet, wrong rmac size, size=%d",*(packet+8));
			/* wrong rmac size */
			return;
		}
		cwrmsg.rmac = packet+8;
	}
	else{
		cwrmsg.rmac=NULL;
	}
#endif


	if (val & CWTH_FLAGS_F){	/* fragmented */
		uint8_t * f;
//		printf("fragman add\n");
		f = fragman_add(conn->fragman, packet,hlen,payloadlen);
		if (f==NULL)
			return;
//		printf("complete\n");
//		printf ("msglen = %i\n",*((uint32_t*)f));
		cwrmsg_init_ctrlhdr(&cwrmsg,f+4,*(uint32_t*)f);
		process_message(conn,&cwrmsg,cb,cbarg); //packet,f+4,*(int32_t*)f);
		free (f);
		return;
	}

	cwrmsg_init_ctrlhdr(&cwrmsg,packet+hlen,len-hlen); //f+4,*(uint32_t*)f);

	process_message(conn,&cwrmsg,cb,cbarg); //packet,f+4,*(int32_t*)f);
	return;
}

