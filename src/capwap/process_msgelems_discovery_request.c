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
int process_msgelems_discovery_request(struct wtpinfo *wtpinfo,int type,uint8_t* msgelem,int len)
{
//	printf("pdiscovery msgelem: %d\n",type);
	uint8_t val;
	switch (type){
		case CWMSGELEM_DISCOVERY_TYPE:
			if (len!=1)
				return 0;
			uint8_t ditype = *((uint8_t*)msgelem);
//			printf("Discovery type: %d\n",ditype);
			break;
		case CWMSGELEM_WTP_BOARD_DATA:
			if (len<4)
				return 0;
			wtpinfo->vendor_id = ntohl(*((uint32_t*)msgelem));
//			printf("Vendor ID %d\n",wtp_info.vendor_id);
			parse_wtp_board_data(wtpinfo,msgelem+4,len-4);

		case CWMSGELEM_WTP_DESCRIPTOR:
			printf("DESCRIPTOR\n");
			if (len<4)
				return;
			 
			val = ntohl(*((uint32_t*)msgelem));
			wtpinfo->max_radios=val>>24;
			wtpinfo->radios_in_use=(val>>16)&0xff;

//			printf("Vendor ID %d\n",wtp_info.vendorid);
//			parse_wtp_board_data(&wtp_info,msgelem+4,len-4);

//			printf("MAX RAD %d, %d\n",wtp_info.max_radios,wtp_info.radios_in_use);


			break;
		
	}
	return 1;
}

*/
