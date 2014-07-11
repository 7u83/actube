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

#include "capwap.h"

int capwap_parse_ctrlhdr(struct capwap_ctrlhdr * ch,uint8_t * msg, int len)
{
	if (len<8)
		return 0;

	uint32_t val;
	ch->msgtype = ntohl(*((uint32_t*)(msg)))-CWIANA_ENTERPRISE_NUMBER*256;
	val = ntohl(*((uint32_t*)(msg+4)));
	ch->seqnum = CW_GET_DWORD_BITS(val,0,8);
	ch->msgelemslen=CW_GET_DWORD_BITS(val,8,16)-3;
	ch->flags=CW_GET_DWORD_BITS(val,24,8);
	ch->msgelems=msg+8;

	if (8+ch->msgelemslen != len)
	{
		return 0;
	}
	return 1;
}


