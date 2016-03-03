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


#ifndef __RADIOINFO_H
#define __RADIOINFO_H

#include "bstr.h"


struct radioinfo{
	char set;
	int rid;
	uint32_t type;
	int admin_state;
	int state;
	int cause;
	bstr_t rmac;

	uint16_t regDomain;

	uint8_t country_str[4];
	uint8_t country_str2[4];

	int cfp_period;
	int cfp_max_duration;
	int beacon_period;
	int dtim_period;
	int max_bssid;
	int occupancy_limit;

	bstr_t bssid;	
	
};

#endif
