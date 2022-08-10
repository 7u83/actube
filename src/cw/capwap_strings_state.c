
#include "capwap.h"

struct cw_StrListElem capwap_strings_state[] = {
	{ CW_STATE_NONE, "None"},	
	{ CAPWAP_STATE_DISCOVERY, "Discovery" },
	{ CAPWAP_STATE_JOIN,"Join" },
	{ CAPWAP_STATE_RUN,"Run" },
	{ CAPWAP_STATE_CONFIGURE,"Configure" },
	{ CW_STATE_IMAGE_DATA,"Image Data" },
	{ CAPWAP_STATE_DTLS_SETUP, "DTLS Setup" },
	{ CAPWAP_STATE_DATA_CHECK, "Data Check" },
	{ CAPWAP_STATE_TIMEOUT, "Timeout"},
	{ CW_STR_STOP,"Undefined" },
};

