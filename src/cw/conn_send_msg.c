#include "conn.h"
#include "cw.h"
#include "dbg.h"

int conn_send_msg(struct conn * conn, uint8_t *rawmsg)
{
	uint8_t * ptr; 
	int packetlen;
	int fragoffset,hlen,mtu;

	packetlen = cw_get_hdr_msg_total_len(rawmsg);

	cw_dbg_msg(DBG_MSG_OUT, conn,rawmsg, packetlen,(struct sockaddr*)&conn->addr);

	/* Zyxel doesn't count msg element length from
	   behind seqnum */
	if (conn->capwap_mode == CW_MODE_ZYXEL){
/*		// XXX val-=3; */
	}


	ptr = rawmsg;

	fragoffset = 0;

	hlen = cw_get_hdr_hlen(rawmsg)*4;

	mtu = conn->mtu;

	while (packetlen>mtu){
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F,1);
		cw_put_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,mtu,(struct sockaddr*)&conn->addr);

		if (conn->write(conn,ptr,mtu)<0)
			return -1;

/*		// XXX Fragmentation stuff.. */
		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		packetlen-=mtu-hlen;

	}


	if (fragoffset)
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F | CAPWAP_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F,0);

	cw_put_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );


	cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,packetlen,(struct sockaddr*)&conn->addr);

	return conn->write(conn,ptr,packetlen-0);
}

