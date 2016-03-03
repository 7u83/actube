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

#include "lwapp.h"
#include "lwmsg.h"


void lwmsg_init(struct lwmsg * lwmsg, uint8_t * buffer, uint8_t * macaddress,int type, int seqnum )
{
	lwmsg->buffer=buffer;
	memcpy (lwmsg->buffer,macaddress,6);
	lwmsg->trnsprthdr=buffer+0; //+6;	
	LWTH_SET_VERSION(lwmsg->trnsprthdr,LW_VERSION);	
	LWTH_SET_C_FLAG(lwmsg->trnsprthdr,1);
	lwmsg->ctrlhdr=buffer+14;
	lwmsg->msgelems=lwmsg->trnsprthdr+6;
	lwmsg->pos=0;	
}
