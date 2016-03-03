/**
 * @file
 * @brief Implementation of lw_readelem_wtp_board_data
 */ 

#include <arpa/inet.h>

#include "log.h"
#include "wtpinfo.h"

#include "lwapp.h"


/**
 * Read LWAPP board data element
 * @param wtpinfo results are stored here
 * @param type type of msg element (must be LWMSGELEM_WTP_BOARD_DATA)
 * @param msgelem msg element data
 * @param len length of msg element
 * @return 0 if msg is not wtp board data\n
 *	1 board data successful read
 */
int lw_readelem_wtp_board_data(struct wtpinfo *wtpinfo, int type, uint8_t *msgelem, int len)
{
	if (type != LW_ELEM_WTP_BOARD_DATA)
		return 0;

	if ( len != 46 ) {
		cw_dbg(DBG_MSG_ERR,"LWAPP msg size wrong. (WTP BOARD DATA) must be 46");
		return -1;
	}

//	uint16_t card_id = htonl( * ( (uint16_t*)(msgelem) ) );
//	uint16_t card_rev = htonl( * ( (uint16_t*)(msgelem +2 ) ) );

	return 1;
}


