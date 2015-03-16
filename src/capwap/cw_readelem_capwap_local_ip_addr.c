#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#
#include "capwap.h"

int cw_readelem_capwap_local_ip_addr(struct sockaddr * local_ip, int type, uint8_t * msgelem, int len)
{
	switch (type){
		case CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS:
		case CWMSGELEM_WTP_IPV4_IP_ADDR:
			{
			if (len!=4)
				return -1;
			struct sockaddr_in  * sain = (struct sockaddr_in*)local_ip;
			memset(sain,0,sizeof(struct sockaddr_in));
			#ifdef HAVE_SIN_LEN
				sain->sa_len=sizeof(struct sockaddr_in);
			#endif
			memcpy(&sain->sin_addr,msgelem,len);	
			sain->sin_family=AF_INET;
			return 1;
			}
#ifdef WITH_IPV6				
		case CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS:
		case CWMSGELEM_WTP_IPV6_IP_ADDR:
			{
			if (len!=16)
				return -1;

			struct sockaddr_in6  * sain = (struct sockaddr_in6*)local_ip;
			memset(sain,0,sizeof(struct sockaddr_in6));
			#ifdef HAVE_SIN6_LEN
				sain->sa_len=sizeof(struct sockaddr_in);
			#endif
			memcpy(&sain->sin6_addr,msgelem,len);
			sain->sin6_family=AF_INET6;


			return 1;
			}
		
#endif				
		}
	return 0;
}


