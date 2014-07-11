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

#include <stdio.h>

#include "sock.h"

void sock_hwaddrtostr(const uint8_t *haddr,int len,char *dst)
{
	int i;
	for (i=0; i<len-1; i++){
		sprintf(dst,"%02X:",haddr[i]);
		dst+=3;
	}
	sprintf(dst,"%02X",haddr[i]);
}


