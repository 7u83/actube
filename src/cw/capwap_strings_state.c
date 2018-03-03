#include "mbag.h"
#include "capwap.h"

struct cw_strlist_elem capwap_strings_state[] = {
{ CAPWAP_STATE_DISCOVERY, "Discovery" },
{ CAPWAP_STATE_JOIN,"Join" },
{ CW_STATE_RUN,"Run" },
{ CW_STATE_CONFIGURE,"Configure" },
{ CW_STATE_IMAGE_DATA,"Image Data" },
{ CW_STR_STOP,"Undefined" },
};

