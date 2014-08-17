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
#include <time.h>

#include "capwap.h"

void cwmsg_addelem_ac_timestamp(struct cwmsg *msg)
{
	time_t t = time(NULL);
	uint32_t ntpt = t+2398291200;
	uint8_t c[4];
       	*((uint32_t*)c)= htonl(ntpt);
	cwmsg_addelem(msg,CWMSGELEM_AC_TIMESTAMP,c,4);
}

