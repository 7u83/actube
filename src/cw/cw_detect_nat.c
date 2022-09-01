
#include <sys/socket.h>

#include "dbg.h"
#include "cw.h"
#include "cfg.h"

/**
 * @brief Detect NAT after a Join Request
 * @param conn Connection object
 * @retval 1 NAT detected
 * @retval 0 no NAT was detected
 */
int cw_detect_nat(struct cw_ElemHandlerParams *params)
{
	const char  * remote_str;
	char local_str[128];

	remote_str = cw_cfg_get(params->cfg,"capwap/local-ip-address",NULL);

	if (remote_str == NULL){
		cw_dbg(DBG_WARN,"Can't detect NAT. No local IP from peer received.");
		return 0;
	}

	/* convert remote connected and sent ip addresse to 
	 * strings */
	 
	sock_addrtostr((struct sockaddr*)&(params->conn->addr),local_str,128,0);
	
	
	/* if connected and sent address is the same, there is 
	 * no NAT */
	if (strcmp(remote_str,local_str)==0){
		cw_dbg(DBG_INFO,"Connection from %s: no NAT detected.",local_str);
		return 0;
	}		
	/* otherwise ther must be something between AC and WTP */
	cw_dbg(DBG_INFO,"Connection from %s: NAT detected.",local_str);
	return 1;
}

