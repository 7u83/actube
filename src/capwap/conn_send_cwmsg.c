/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <string.h>

#include "conn.h"
#include "capwap.h"

int conn_send_cwmsg(struct conn * conn, struct cwmsg * cwmsg) 
{
	uint32_t val;

	/* second dword of message control header */
	val = (cwmsg->seqnum<<24)|((cwmsg->pos+3)<<8);
	*((uint32_t*)(cwmsg->ctrlhdr+4))=htonl(val);

	
//	int msglen = cwmsg->ctrlhdr-cwmsg->buffer+cwmsg->pos;
	int msglen = cwmsg->pos+8;

	uint8_t * ptr = cwmsg->buffer;
	int fragoffset = 0;
	int hlen = cwmsg->hlen*4;

//	printf("Hlen is %i\n",hlen);


	int preamble = CW_VERSION | 0;


	int wbid=CWTH_WBID_IEEE80211;
	int packetlen = msglen+hlen;	
	int mtu = conn->mtu;

	while (packetlen>mtu){
		val = (preamble << 24) | ((hlen/4)<<19) | (cwmsg->rid<<14) |(wbid<<9) | 
				CWTH_FLAGS_T |
				CWTH_FLAGS_F |
				cwmsg->flags;
		*((uint32_t*)ptr)=htonl(val);

//		printf("setting frag id : %i\n",conn->fragid);
		val = conn->fragid<<16 | fragoffset<<3;
//		printf("VAAL: %08X\n",val);
		*((uint32_t*)(ptr+4))=htonl(val);

//		printf("Offset = %d\n",fragoffset);

//		if (conn_send_packet(conn,ptr,mtu)<0)
//			return -1;	
		if (conn->write(conn,ptr,mtu)<0)
			return -1;

		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		packetlen-=mtu-hlen;
//		printf("Packelen=%d\n",packetlen);

		if (hlen>8)
			memcpy(ptr+8,cwmsg->ctrlhdr+8,hlen-8);

	}

	val = (preamble << 24) | ((hlen/4)<<19) | (cwmsg->rid<<14) |(wbid<<9) | 
		CWTH_FLAGS_T|cwmsg->flags;

	if (fragoffset){
		val |= CWTH_FLAGS_F | CWTH_FLAGS_L;
	}		


//	printf("Setting first byte %08X\n",val);
	*((uint32_t*)ptr)=htonl(val);


	val = conn->fragid<<16 | fragoffset<<3;
	*((uint32_t*)(ptr+4))=htonl(val);

	return conn->write(conn,ptr,msglen-fragoffset*8+hlen);
}

