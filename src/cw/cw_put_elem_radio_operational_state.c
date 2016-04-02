
#include "cw.h"

/**
 * Put an Operational State message element to a buffer.
 * @param dst Destination buffer
 * @param rid Radio ID
 * @param os Operational State
 * @param d7mode puts the Operational Stae element in CAPWAP draft 7 mode.
 * @return Number of byes put
 */
int cw_put_elem_radio_operational_state(uint8_t * dst, int rid, int os, int d7mode)
{
	uint8_t *d=dst;

	/* Put the radio ID */
	cw_put_byte(d+4,rid);

	if ( d7mode ){
		/* Isolate Oper Sate from cause */
		uint8_t o=os>>8; 

		/* Invert oper state for Cisco, if oper state is 2 or 1 */
		if (o!=0 && o<=2) {
			/*  2 becomes 1 and 1 becomes 2 */
			os = (os & 0x00ff ) | ((3-o)<<8);
		}
	}

	/* Put oper state */
	cw_put_word(d+5,os);
	d+=3+cw_put_elem_hdr(d,CW_ELEM_RADIO_OPERATIONAL_STATE,3);

	return d-dst;
}

