/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
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


#include "capwap.h"
#include "wtpinfo.h"


int cw_readelem_wtp_reboot_statistics(struct wtp_reboot_statistics *s, int type,uint8_t *msgelem, int len)
{
	if (type != CW_ELEM_WTP_REBOOT_STATISTICS)
		return 0;

	s->reboot_count=ntohs(*((uint16_t*)(msgelem+0)));
	s->ac_initiated_count=ntohs(*((uint16_t*)(msgelem+2)));
	s->link_failure_count=ntohs(*((uint16_t*)(msgelem+4)));
	s->sw_failure_count=ntohs(*((uint16_t*)(msgelem+6)));
	s->hw_failure_count=ntohs(*((uint16_t*)(msgelem+8)));
	s->other_failure_count=ntohs(*((uint16_t*)(msgelem+10)));
	s->unknown_failure_count=ntohs(*((uint16_t*)(msgelem+12)));
	s->last_failure_type=*(msgelem+14);

	return 1;	
}

