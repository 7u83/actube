
#include "capwap.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"
#include "radio.h"

int cw_out_radio_operational_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	uint8_t *d=dst;

	/* Iterate through all radios and send the CW_RADIO_OPER_STATE item if found.
	   We assume, that any other processes, dealing with setting the 
	   the Radio Admin state, adds a CW_RADIO_OPER_STATE item to the radio, 
	   depending on results. */

	int nerror=0;
	
	MAVLITER_DEFINE(it,conn->radios);
	mavliter_foreach(&it){
		mbag_item_t * radioitem = mavliter_get(&it);
		mbag_item_t *ositem = mbag_get(radioitem->data,CW_RADIO_OPER_STATE);
		if (!ositem){
			nerror++;
			continue;
		}

		/* Put the radio ID */
		cw_put_byte(d+4,radioitem->id);
		
		/* Get the operational state and cause */
		uint16_t os = ositem->word;

		if ( conn->capwap_mode==CW_MODE_CISCO ){
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

		/* delete the operational state item, so it won't be 
		   sent again, until it is set again by a change through
		   Set Radio Admin State */
		mbag_del(radioitem->data,CW_RADIO_OPER_STATE);

	}

	if (nerror) {
		if (a->mand) {
			cw_log(LOG_ERROR,"Could not send Radio Operational State for all radios. Sent %d out of %d.",
				conn->radios->count-nerror,conn->radios->count);

		}
	}

	return d-dst;
}

