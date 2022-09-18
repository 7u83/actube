#include "conn.h"
#include "cw.h"
#include "dbg.h"

#define CW_MODE_ZYXEL 7
extern int cw_send_msg(struct cw_Conn * conn,uint8_t * msg);

int conn_send_msg(struct cw_Conn * conn, uint8_t *rawmsg)
{
	uint8_t * ptr; 
	int packetlen;
	int fragoffset,hlen,mtu;

	packetlen = cw_get_hdr_msg_total_len(rawmsg);

	cw_dbg_msg(DBG_MSG_OUT, conn,rawmsg, packetlen,(struct sockaddr*)&conn->addr);
	{
	int type;
	uint8_t *msgptr;
	msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);
	struct cw_MsgData * msg;
	type = cw_get_msg_type(msgptr);
	msg = cw_msgset_get_msgdata(conn->msgset,type);
	uint8_t *elems_ptr;

	int offset = cw_get_hdr_msg_offset(rawmsg);

	uint8_t *msg_ptr = rawmsg + offset;
	int elems_len = cw_get_msg_elems_len(msg_ptr);
	elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	cw_Cfg_t * cfg = cw_cfg_create();
	cfg->dbg_level = DBG_ELEM_DETAIL_OUT;
	cfg->dbg_prefix = "    ";

	struct cw_ElemHandlerParams params;
	memset(&params,0,sizeof(struct cw_ElemHandlerParams));
	
	params.cfg=cfg;
	params.msgset=conn->msgset;
	params.msgdata=msg;
	params.mand_found = mavl_create_conststr();
	params.dbg_level = DBG_ELEM_OUT;

	cw_decode_elements( &params, elems_ptr,elems_len);
	cw_cfg_destroy(cfg);
	if (params.mand_found){
		cw_check_missing_mand(msg, params.mand_found,conn->msgset->handlers_by_key);
		mavl_destroy(params.mand_found);
	}

	}




	//  Zyxel doesn't count msg element length from
//	   behind seqnum * /
//	if (conn->capwap_mode == CW_MODE_ZYXEL){
// *		// XXX val-=3; * /
//	}


	ptr = rawmsg;

	fragoffset = 0;

	hlen = cw_get_hdr_hlen(rawmsg)*4;

	mtu = conn->mtu;
mtu = 9440;	
	mtu = mtu >> 3;
	mtu = mtu << 3;
	
//	printf("packetlenX = %d (%d)\n",packetlen,hlen);
	 int offset = cw_get_hdr_msg_offset(rawmsg);
	return cw_send_msg(conn,rawmsg+offset);

/*
	if (packetlen>mtu){
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_F,1);
		cw_set_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );
		cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,mtu,(struct sockaddr*)&conn->addr);
		if (conn->write(conn,ptr,mtu)<0)
			return -1;
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_M,0);
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_W,0);

		fragoffset+=(mtu-hlen)/8;
		hlen = 8;
		ptr += mtu-hlen;
		packetlen-=mtu-hlen;
		memcpy(ptr,rawmsg,hlen);

	}

	while (packetlen>mtu){
		
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_F,1);
		cw_set_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,mtu,(struct sockaddr*)&conn->addr);

		if (conn->write(conn,ptr,mtu)<0)
			return -1;

/ *		// XXX Fragmentation stuff.. * /
		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		packetlen-=mtu-hlen;
		memcpy(ptr,rawmsg,hlen);
	}


	if (fragoffset)
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_F | CAPWAP_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(ptr,CAPWAP_FLAG_HDR_F,0);

	cw_set_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );


	cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,packetlen,(struct sockaddr*)&conn->addr);

	return conn->write(conn,ptr,packetlen-0); */
}

