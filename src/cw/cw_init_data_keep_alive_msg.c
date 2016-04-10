#include "cw.h"
#include "capwap.h"




uint8_t *cw_init_data_keep_alive_msg(uint8_t * buffer,uint8_t *rmac)
{
	/* zero the first 8 bytes */
	memset(buffer,0,8);

	/* put capwap header, unencrypted */
#if CAPWAP_PACKET_PREAMBLE != 0
	cw_set_hdr_preamble(buffer, CAPWAP_PACKET_PREAMBLE);
#endif

	cw_set_hdr_rmac(buffer, rmac);

	cw_set_hdr_flags(buffer, CW_FLAG_HDR_K,1);

	return cw_get_hdr_payload_ptr(buffer);
}

