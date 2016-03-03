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
#include "sock.h"



int hdr_print(char *str, uint8_t *packet, int len)
{
	

	char *s = str;


	if (len<1){
		s+=sprintf(s,"\tNo info available.");
		return s-str;
	}

	int preamble = cw_get_hdr_preamble(packet); //CWTH_GET_PREAMBLE(packet);

	if (preamble==01){
		s+=sprintf(s,"\tEncrypted data.");
		return s-str;
	}

	if (preamble!=00){
		s+=sprintf(s,"\tWrong CAPWAP version or encryption type.");
		return s-str;
	}


	if (len < 4){
		s+=sprintf(s,"\tNo more data. Packet too short.");
		return s-str;
	}
	


	int hlen = cw_get_hdr_hlen(packet); //CWTH_GET_HLEN(packet);
	int rid = cw_get_hdr_rid(packet);
	int wbid = cw_get_hdr_wbid(packet);
	s+=sprintf(s,"\tHLEN: %d, RID: %02X, WBID %02X",hlen,rid,wbid);	



	s+=sprintf(s," Flags: (T=%d,F=%d,L=%d,W=%d,M=%d,K=%d)\n",
				cw_get_hdr_flag_t(packet),
				cw_get_hdr_flag_f(packet),
				cw_get_hdr_flag_l(packet),
				cw_get_hdr_flag_w(packet),
				cw_get_hdr_flag_m(packet),
				cw_get_hdr_flag_k(packet)
		);
	if (cw_get_hdr_flag_m(packet)){
		uint8_t * rmac = cw_get_hdr_rmac(packet);
		s+=sprintf(s,"\tRadio MAC: %s\n",sock_hwaddr2str(bstr_data(rmac),
				bstr_len(rmac)));

	}


	if (len < 8){
		s+=sprintf(s,"\tNo more data. Packet too short.");
		return s-str;
	}
	
	int frag_id = cw_get_hdr_fragid(packet);
	int frag_offs = cw_get_hdr_fragoffset(packet);
	s+=sprintf(s,"\tFrag Id: %d, Frag Offs:: %d\n",frag_id,frag_offs);


	int bhlen = 4*hlen;
	if (cw_get_hdr_flag_f(packet) && frag_offs!=0){
		s+=sprintf(s,"\tFragment data ...");
		return s-str;
	}

	
	if (len<bhlen+4){
		s+=sprintf(s,"\tNo more data. Packet too short.");
		return s-str;
	}


	int msgtype = ntohl(*((uint32_t*)(packet+bhlen)));
	s+=sprintf(s,"\tMsgType: %d",msgtype);

	if (len<bhlen+8){
		s+=sprintf(s,"\n\tNo more data. Packet too short.");
		return s-str;
	}

	int seqnum = (ntohl(*((uint32_t*)(packet+bhlen+4))))>>24;
	int msgelemlen = 0xFF & ((ntohl(*((uint32_t*)(packet+bhlen+4))))>>8);

	s+=sprintf(s,", SeqNum: %d, MsgelemLen:%d",seqnum,msgelemlen);


	return s-str;
}
