#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "wtpinfo.h"
#include "capwap.h"

#include "log.h"


/*
int wtpinfo_readelem_ecn_support(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_ECN_SUPPORT)
		return 0;

	if (len!=1){
		cw_log_debug0("Discarding ECN_SUPPORT msgelem, wrong size, type=%d,len=%d",type,len);
		return 1;
	}
	wtpinfo->ecn_support=*msgelem;
	return 1;

}
*/

int cw_readelem_cw_local_ip_addr(struct sockaddr * local_ip, int type, uint8_t * msgelem, int len)
{
	switch (type){
		case CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS:
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
		case CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS:
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


int wtpinfo_readelem_cw_local_ip_addr(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{

	switch (type){
		case CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS:
			{
			if (len!=4)
				return -1;
			struct sockaddr_in  * sain = (struct sockaddr_in*)&wtpinfo->local_ip;
			memset(sain,0,sizeof(struct sockaddr_in));
			#ifdef HAVE_SIN_LEN
				sain->sa_len=sizeof(struct sockaddr_in);
			#endif
			memcpy(&sain->sin_addr,msgelem,len);	
			sain->sin_family=AF_INET;
			return 1;
			}
#ifdef WITH_IPV6				
		case CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS:
			{
			if (len!=16)
				return -1;

			struct sockaddr_in6  * sain = (struct sockaddr_in6*)&wtpinfo->local_ip;
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


