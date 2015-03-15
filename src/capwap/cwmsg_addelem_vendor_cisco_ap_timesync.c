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
#include "capwap_cisco.h"


void cwmsg_addelem_vendor_cisco_ap_timesync(struct cwmsg *cwmsg)
{
	//time_t t = htonl(time(NULL));
	uint32_t t = htonl(time(NULL));
	cwmsg_addelem_vendor_specific_payload(cwmsg, CW_VENDOR_ID_CISCO, CWVENDOR_CISCO_AP_TIMESYNC,
					      (uint8_t *) & t, sizeof(t));
}
