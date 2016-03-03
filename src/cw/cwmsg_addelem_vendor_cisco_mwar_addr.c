#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>


#include "capwap.h"
#include "capwap_cisco.h"

#include "cwmsg.h"
#include "conn.h"


#include "sock.h"

void cwmsg_addelem_vendor_cisco_mwar_addr(struct cwmsg *msg, struct conn *conn)
{
	uint8_t data [7];

	switch (((struct sockaddr*)&conn->addr)->sa_family){
		case AF_INET:
		{
			data[0]=2;	/* mwar type */
			data[5]=0;
			data[6]=0;

printf("Adding mwar addr %s\n",sock_addr2str(&conn->addr));

			struct sockaddr_in  * sain = (struct sockaddr_in*)&conn->addr;
			memcpy(data+1,(uint8_t*)&sain->sin_addr,4);
			cwmsg_addelem_vendor_specific_payload(msg, CW_VENDOR_ID_CISCO, 
						CW_CISCO_MWAR_ADDR,
						data,7);


			break;
		}


/*		case AF_INET6:
		{
			int id;
			if (cw_mode == CWMODE_CISCO)
				id = CWMSGELEM_WTP_IPV6_IP_ADDR; 
			else
				id = CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS;
			struct sockaddr_in6  * sain = (struct sockaddr_in6*)&a;
			return cwmsg_addelem(msg,id,(uint8_t*)&sain->sin6_addr,16);
		}
		break;
*/
	}




}
