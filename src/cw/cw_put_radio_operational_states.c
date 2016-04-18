/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 * @file
 * @brief Implementation of cw_put_radio_operational_states
 */

#include "cw.h"
#include "capwap_items.h"
#include "radio.h"


/**
 * Put all radio operational states 
 */
int xxcw_put_radio_operational_states(mbag_t radios, uint8_t * dst, int *nerror, int d7mode)
{
	uint8_t *d=dst;

	/* Iterate through all radios and send the CW_RADIO_OPER_STATE item if found.
	   We assume, that any other processes, dealing with setting the 
	   the Radio Admin state, adds a CW_RADIO_OPER_STATE item to the radio, 
	   depending on results. */

//	int nerror=0;
	
	*nerror=0;

	MAVLITER_DEFINE(it,radios);
	mavliter_foreach(&it){

		mbag_item_t * radioitem = mavliter_get(&it);
		mbag_item_t *ositem = mbag_get(radioitem->data,CW_RADIOITEM_OPER_STATE);
		if (!ositem){
			(*nerror)++;
			continue;
		}


		/* Put the radio ID */
		cw_put_byte(d+4,radioitem->iid);
		
		/* Get the operational state and cause */
		uint16_t os = ositem->word;

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

		/* delete the operational state item, so it won't be 
		   sent again, until it is set by a change through
		   Set Radio Admin State */
		mbag_del(radioitem->data,CW_RADIOITEM_OPER_STATE);

	}


/*	if (nerror) {
		if (a->mand) {
			cw_log(LOG_ERROR,"Could not send Radio Operational State for all radios. Sent %d out of %d.",
				conn->radios->count-nerror,conn->radios->count);

		}
	}
*/
	return d-dst;
}


