#ifndef __CW_RADIO_H
#define __CW_RADIO_H

#include "mbag.h"

enum radiodata{
	CW_RADIO_ADMIN_STATE=10000,
	CW_RADIO_TYPE,
	CW_RADIO_REG_DOMAIN,
	CW_RADIO_BSSID,
	CW_RADIO_SHORT_PREAMBLE,
	CW_RADIO_COUNTRY_STRING,

/* Cisco */

	CW_RADIO_OCCUPANCY_LIMIT,
	CW_RADIO_CFP_PERIOD,
	CW_RADIO_CFP_MAX_DURATION


};


#endif
