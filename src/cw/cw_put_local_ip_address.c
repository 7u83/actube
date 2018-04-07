#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"
#include "dbg.h"
#include "cw.h"

int cw_put_local_ip_address(uint8_t *dst, int id, int ipv4_id, int ipv6_id, 
				uint8_t *src, int len)
{

	if (len ==4 && id == ipv4_id){
		cw_put_data(dst + 4, src, 4);
		return 4 + cw_put_elem_hdr(dst, id, 4);
	}

	if (len ==16 && id == ipv6_id){
		
		cw_put_data(dst + 4, src, 16);
		return 4 + cw_put_elem_hdr(dst, id, 4);
	}
	return 0;
}







