
#include <sys/socket.h>

#include "dbg.h"
#include "cw.h"

/**
 * @brief Detect NAT after a join/discovery request
 * @param conn Connection object
 * @retval 1 NAT detected
 * @retval 0 no NAT was detected
 */
int cw_detect_nat(struct conn *conn)
{
	cw_KTV_t * result;
	
	char local_str[128];
	char remote_str[128];

	result = cw_ktv_get(conn->remote_cfg,"capwap-local-ip-address",CW_TYPE_IPADDRESS);
	if (result == NULL){
		cw_dbg(DBG_WARN,"Can't detect NAT. No local IP from peer received.");
		return 0;
	}

	/* convert remote connected and sent ip addresse to 
	 * strings */
	 
	sock_addrtostr((struct sockaddr*)&conn->addr,local_str,128,0);
	result->type->to_str(result,remote_str,128);
	
	
	/* if connected and sent address is the same, there is 
	 * no NAT */
	if (strcmp(remote_str,local_str)==0)
		return 0;
	/* otherwise ther must be something between AC and WTP */
	return 1;
}
