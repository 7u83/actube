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

/*
 * for each capwap message element in msgelems call the callback function
 */
int process_msgelems(uint8_t * msgelems,  int len,
		int (*callback)(void*,int,uint8_t*,int),void *arg )
{
//	printf("NUNU %08X\n",msgelems);

	uint32_t val;
	int type;
	int elen;
	int i=0;
	do {
		val = ntohl(*(uint32_t*)(msgelems+i));
		type=(val>>16) & 0xFFFF;
		elen = val & 0xffff;
		if (i+elen+4>len) {
//			printf("Error AAA");
//			exit(88);
			return 0;
		}
			
		callback(arg,type,msgelems+i+4,elen);
//		process_joinmsgelem(wtpman,msgelems+i+4,type,elen);
//		printf("type %d, len %d\n",type,elen);
		i+=elen+4;
//		printf("I %d, len %d\n",i,len);

	} while (i<len);
	return 1;
}


