
#include "conn.h"
#include "capwap.h"
#include "dbg.h"
#include "sock.h"


int conn_send_msg(struct conn * conn, uint8_t *rawmsg)
{

	int packetlen = cw_get_hdr_msg_total_len(rawmsg);

	cw_dbg_msg(DBG_MSG_OUT, conn,rawmsg, packetlen,(struct sockaddr*)&conn->addr);


	//uint8_t * msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);


	/* Zyxel doesn't count msg element length from
	   behind seqnum */
	if (conn->capwap_mode == CW_MODE_ZYXEL){
		// XXX val-=3;
	}


	uint8_t * ptr = rawmsg;

	int fragoffset = 0;

	int hlen = cw_get_hdr_hlen(rawmsg)*4;

//cw_set_hdr_hlen(rawmsg,223);
//*(rawmsg + 8)=99;
//cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_M,1);

	int mtu = conn->mtu;


	while (packetlen>mtu){
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F,1);
		cw_put_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,mtu,(struct sockaddr*)&conn->addr);
		//XXX
		{
//			char h[200];
//			hdr_print(h,ptr,mtu);
//			cw_dbg(DBG_PKT_OUT,"Sending capwap packet to %s:\n%s",sock_addr2str(&conn->addr),h);
		}
//		cw_dbg_dmp(DBG_PKT_DMP,ptr,mtu,"Sending packet ...");
		

		if (conn->write(conn,ptr,mtu)<0)
			return -1;

		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		packetlen-=mtu-hlen;

//XXX		if (hlen>8)
//			memcpy(ptr+8,cwmsg->ctrlhdr+8,hlen-8);

	}

//	val = (preamble << 24) | ((hlen/4)<<19) | (cwmsg->rid<<14) |(wbid<<9) | 
		/*CWTH_FLAGS_T|*/ //cwmsg->flags;


//printf("VAL = %08x, %08x\n",val,cwmsg->flags);

//printf("FRag offset :::::::::::::::::::::::::::: %d\n",fragoffset);

	if (fragoffset)
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F | CW_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F,0);


//	printf("Setting first byte %08X\n",val);
//	*((uint32_t*)ptr)=htonl(val);


//	val = conn->fragid<<16 | fragoffset<<3;
//	*((uint32_t*)(ptr+4))=htonl(val);

	cw_put_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );


	{
	//	char h[1024];
	//	hdr_print(h,ptr,msglen-fragoffset*8+hlen);
	//	cw_dbg(DBG_CW_PKT_OUT,"Sending capwap packet to %s:\n%s",sock_addr2str(&conn->addr),h);
	}


//	cw_dbg_dmp(DBG_PKT_DMP,ptr,packetlen,"Sending packet ...");
	//return conn->write(conn,ptr,msglen-fragoffset*8+hlen);



//printf("Send packet len %p %d\n",ptr,packetlen);
	cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,packetlen,(struct sockaddr*)&conn->addr);

	return conn->write(conn,ptr,packetlen-0);
}

