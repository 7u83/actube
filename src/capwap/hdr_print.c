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


#include <stdint.h>
#include <stdio.h>

#include "capwap.h"




int hdr_print(char *str, uint8_t *packet, int len)
{

	char *s = str;
	int hlen = CWTH_GET_HLEN(packet);
	int rid = CWTH_GET_RID(packet);
	int wbid = CWTH_GET_WBID(packet);
	s+=sprintf(s,"\tHLEN: %d, RID: %02X, WBID %02X",hlen,rid,wbid);	



	s+=sprintf(s," Flags: (T=%d,F=%d,L=%d,W=%d,M=%d,K=%d)\n",
				CWTH_GET_FLAG_T(packet),
				CWTH_GET_FLAG_F(packet),
				CWTH_GET_FLAG_L(packet),
				CWTH_GET_FLAG_W(packet),
				CWTH_GET_FLAG_M(packet),
				CWTH_GET_FLAG_K(packet)
		);

	int frag_id = CWTH_GET_FRAGID(packet);
	int frag_offs = CWTH_GET_FRAGOFFSET(packet);


	int bhlen = 4*hlen;
	int msgtype = ntohl(*((uint32_t*)(packet+bhlen)));
	int seqnum = (ntohl(*((uint32_t*)(packet+bhlen+4))))>>24;
	int msgelemlen = 0xFF & ((ntohl(*((uint32_t*)(packet+bhlen+4))))>>8);

	s+=sprintf(s,"\tFrag Id: %d, Frag Offs:: %d\n",frag_id,frag_offs);
	s+=sprintf(s,"\tMsgType: %d, SeqNum: %d, MsgelemLen:%d\n",msgtype,seqnum,msgelemlen);





	return s-str;
}
