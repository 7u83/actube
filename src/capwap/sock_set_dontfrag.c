
#include <netinet/in.h>
#include <sys/types.h>          
#include <sys/socket.h>



int sock_set_dontfrag(int sock,int val)
{
#if defined IP_MTU_DISCOVER && defined IP_PMTUDISC_DONT
	int mtu_type;
	if (val)
       		mtu_type = IP_PMTUDISC_DO;
	else
		mtu_type = IP_PMTUDISC_WANT;

	return setsockopt(sock, SOL_IP, IP_MTU_DISCOVER, &mtu_type, sizeof (mtu_type));
#elif defined IP_DONTFRAG

	const int opt = val;
	return setsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &opt, sizeof (opt));

#endif

}
