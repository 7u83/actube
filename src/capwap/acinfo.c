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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#include "capwap.h"
#include "cw_log.h"
#include "cw_util.h"

#include "acinfo.h"

#include "sock.h"


/*
int acinfo_readelem_ac_name(struct ac_info * acinfo,int type, uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_AC_NAME)
		return 0;
	cw_setstr(&acinfo->ac_name,msgelem,len);
	return 1;
}
*/

int acinfo_readelem_ac_descriptor(struct ac_info * acinfo,int type, uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_AC_DESCRIPTOR)
		return 0;

	if (len<12)
		return -1;

	uint32_t val;

	/* read stations and limit */
	val = ntohl(*((uint32_t*)msgelem));
	acinfo->stations  = val>>16;
	acinfo->limit = val&0xffff;


	/* read active wtps and max wtps */
	val = ntohl(*((uint32_t*)(msgelem+4)));
	acinfo->active_wtps  = val>>16;
	acinfo->max_wtps = val&0xffff;

	/* read active wtps and max wtps */
	val = ntohl(*((uint32_t*)(msgelem+8)));
	acinfo->security = val >>24;
	acinfo->rmac= (val >> 16)&0xff;
	acinfo->dtls_policy=val&0xf;

	int sub=12;
	int sublen;
	while (sub<len){
		if (len-sub<8)
			return -1;

		uint32_t vendor = ntohl(*((uint32_t*)(msgelem+sub)));
		val = ntohl(*((uint32_t*)(msgelem+sub+4)));
		sublen = val&0xffff;
		sub+=8;
		if (vendor != 0){
			sub+=sublen;
			continue;
		}
		int subtype = val>>16;

		if (sub+sublen>len)
			return -1;


		switch (subtype){
			case 4:
				cw_setstr(&acinfo->hardware_version,msgelem+sub,sublen);
				break;
			case 5:
				cw_setstr(&acinfo->software_version,msgelem+sub,sublen);
				break;
		}
		sub+=sublen;

	}


	return 1;	
}




int acinfo_readelem_ctrl_ip_addr(struct ac_info * acinfo, int type, uint8_t*msgelem,int len)
{
	switch (type){
		case CWMSGELEM_CONTROL_IPV4_ADDRESS:
		{
			if (len!=6)
				return -1;

			if (!acinfo->aciplist)
				return 1;

		
			ACIP * acip = malloc(sizeof(ACIP));
			if (!acip) {
				cw_log(LOG_ERR,"Can't allocate memory for acv4ip: %s",strerror(errno));
				return 1;
			}

			struct sockaddr_in addr;
			memcpy(&addr.sin_addr,msgelem,4);
			addr.sin_family=AF_INET;
			memcpy(&acip->ip,&addr,sizeof(addr));

			acip->wtp_count = ntohs( * ((uint16_t*)(msgelem+4)));
			aciplist_add(acinfo->aciplist,acip);

//			printf ("Read AC IP Elem %s\n",sock_addr2str(&acip->ip));
//			printf ("WTP COUNT HERE %d\n",acip->wtp_count);

			return 1;

		}
		case CWMSGELEM_CONTROL_IPV6_ADDRESS:
		{
			if (len!=18)
				return -1;

			if (!acinfo->aciplist)
				return 1;

		
			ACIP * acip = malloc(sizeof(ACIP));
			if (!acip) {
				cw_log(LOG_ERR,"Can't allocate memory for acv4ip: %s",strerror(errno));
				return 1;
			}

			struct sockaddr_in6 addr;
			memcpy(&addr.sin6_addr,msgelem,16);
			addr.sin6_family=AF_INET6;
			memcpy(&acip->ip,&addr,sizeof(addr));
			acip->wtp_count = ntohs( * ((uint16_t*)(msgelem+16)));

			aciplist_add(acinfo->aciplist,acip);

			return 1;
		}

	}

	return 0;
}



