
#include <stdint.h>
#include <time.h>


#include "cw/msgset.h"
#include "cw/cw.h"




int cisco_out_ap_timesync(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	cw_set_dword(dst+10 , time(NULL));
        cw_set_byte(dst+10 + 4, 0);

	return 5 + cw_put_elem_vendor_hdr(dst, eh->vendor , eh->id, 5);
}

