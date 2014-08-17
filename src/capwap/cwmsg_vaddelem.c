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
#include <stdarg.h>
#include <arpa/inet.h>

#include "cwmsg.h"

void cwmsg_vaddelem(struct cwmsg *msg,int type,int n, ...)
{
	va_list vl;
	va_start(vl,n);
	int i,len=0;
	for (i=0; i<n; i++){

		uint8_t *data=va_arg(vl,uint8_t*);
		int l=va_arg(vl,int);
		memcpy(msg->msgelems+4+msg->pos+len,data,l);
		len+=l;
	}

	uint32_t val = type<<16|len;
	*((uint32_t*)(msg->msgelems+msg->pos))=htonl(val);
	msg->pos+=4+len;
	va_end(vl);
}
