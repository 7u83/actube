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

#include <stdio.h>

#include "cw_util.h"

/**
 *@file
 *@brief Implementation of cw_sprintf_version 
 */


/**
 * Converts a CAPWAP version string to readable format.
 * @param s target where the formated string will be stored
 * @param ver the CAPWAP version data
 * @param vendor vendor id 
 * @param def default string will be printed if version is NULL
 * @return the number bytes added to the output *s
 *
 * This function tries to determine if the bstr_t string version 
 * is human readable, and if not, it will put out a hexadecimal value
 * or a dot-seaprated list of number for version elements used
 * by Cisco.
 */
int cw_format_version(char *s, bstr_t ver, uint32_t vendor, char * def)
{
	uint8_t * version = bstr_data(ver);
	int len = bstr_len(ver);

	
	if (!version)
		return sprintf(s,"%s",def);


	int rs=0;	
	int i;


	if ( cw_is_printable(version,len)  ){
		if (len != 0 )
			rs+=sprintf(s+rs,"%s",version);
		else
			rs+=sprintf(s+rs,"''");
	}
	else{
		for (i=0; i<len && i<20; i++){
			rs+=sprintf(s+rs,"%02X",version[i]);
		}

		int dot=0;

		rs+=sprintf(s+rs," (");
		for (i=0; i<len && i<20; i++){
			if (dot) 
				rs+=sprintf(s+rs,".");
			dot=1;
			rs+=sprintf(s+rs,"%d",version[i]);
		}
		rs+=sprintf(s+rs,")");
	}


	rs+=sprintf(s+rs,", Vendor Id: %d, %s",vendor, cw_ianavendoridtostr(vendor));
	rs+=sprintf(s+rs,"\n");
	return rs;	
}

