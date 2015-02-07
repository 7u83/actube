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
#include <netinet/in.h>

#include "capwap.h"
#include "acinfo.h"
#include "cw_log.h"


#include "sock.h"

//static int cwmsg_addelem_acip(struct cwmsg * msg,ACIP *ip,int ctr)
static int cwmsg_addelem_acip(void * priv,void *data) //,int ctr)
{
//	cw_log_debug2("Adding Ctrl IP %s",sock_addr2str((struct sockaddr*)data));

	struct cwmsg * msg = (struct cwmsg*)priv;
	ACIP * acip = (ACIP*)data;

	uint8_t ipmsg [18];

	switch (acip->ip.ss_family){
		case AF_INET:
		{
			struct sockaddr_in  * sain = (struct sockaddr_in*)&acip->ip;
			memcpy(ipmsg,&sain->sin_addr.s_addr, sizeof(sain->sin_addr.s_addr));
			*((uint16_t*)(ipmsg+4))= htons(acip->wtp_count); /* number of wtps */
			cwmsg_addelem(msg,CWMSGELEM_CONTROL_IPV4_ADDRESS,ipmsg,6);
		}
		break;
#ifdef WITH_IPV6				
		case AF_INET6:
		{
			struct sockaddr_in6  * sain = (struct sockaddr_in6*)&acip->ip;
			memcpy(ipmsg,&sain->sin6_addr.s6_addr, sizeof(sain->sin6_addr.s6_addr));
			*((uint16_t*)(ipmsg+16))= htons(acip->wtp_count); /* number of wtps */
			cwmsg_addelem(msg,CWMSGELEM_CONTROL_IPV6_ADDRESS,ipmsg,18);
		}
		break;
#endif				
	}
	return 1;
}


void cwmsg_addelem_ctrl_ip_addrs(struct cwmsg *msg, struct ac_info *acinfo)
{
//	printf("Counter in the list: %i\n",acinfo->aciplist->count);
	aciplist_foreach(acinfo->aciplist,cwmsg_addelem_acip,msg);
}


/*

void ucwmsg_addelem_ctrl_ip_addrs(struct cwmsg *msg, struct ac_info *acinfo)
{
	int i;


	for (i=0; i<acinfo->salist_len; i++)
	{
		uint8_t ipmsg [18];
		switch (acinfo->salist[i].sa_family){
			case AF_INET:
			{
				struct sockaddr_in  * sain = (struct sockaddr_in*)&acinfo->salist[i];
				memcpy(ipmsg,&sain->sin_addr, sizeof(sain->sin_addr));
				*((uint16_t*)(ipmsg+4))= htons(0); // number of wtps 
				cwmsg_addelem(msg,CWMSGELEM_CONTROL_IPV4_ADDRESS,ipmsg,6);
			}
			break;
#ifdef WITH_IPV6				
			case AF_INET6:
			{
				struct sockaddr_in6  * sain = (struct sockaddr_in6*)&acinfo->salist[i];
				memcpy(ipmsg,&sain->sin6_addr, sizeof(sain->sin6_addr));
				*((uint16_t*)(ipmsg+16))= htons(0); // number of wtps 
				cwmsg_addelem(msg,CWMSGELEM_CONTROL_IPV6_ADDRESS,ipmsg,18);
			}
			break;
#endif				
		}
	}
}

*/
