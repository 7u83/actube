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

#include "cw.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"
#include "radio.h"


int xxcw_out_radio_operational_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	int nerror=0;
	int l = cw_put_radio_operational_states(conn->radios,dst,&nerror,0);
	if (nerror) {
		if (a->mand) {
			cw_log(LOG_ERROR,"Could not send Radio Operational State for all radios. Sent %d out of %d.",
				conn->radios->count-nerror,conn->radios->count);

		}
	}
	return l;
}

