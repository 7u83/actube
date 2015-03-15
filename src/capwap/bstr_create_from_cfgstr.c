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

/**
 * @file
 * @brief bstr_create_from_cfgstr function
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bstr.h"


/**
 * Create a bstr_t string from a string read from config file.
 * 
 * @param s String from config file.
 * @return The create bstr_t string.
 *
 * The string from config file is an ASCII-text which is interpreted
 * as hexadecimal string if it starts with ".x"
 *
 * @see bstr_t
 */
uint8_t * bstr_create_from_cfgstr(const char * s)
{
	int l = strlen(s);

	
	if (s[0]!='.')
		return bstr_create((uint8_t*)s,l);

	if (l<=2)
		return bstr_create((uint8_t*)s,l);

	if (s[1]=='.')
		return bstr_create((uint8_t*)s+1,l-1);

	if (s[1]=='x'){
		uint8_t * ns=0;
		int len=0;

		int ch,cl;
		const char *ss = s+2;
		int rc ;
		do {
			rc = sscanf(ss,"%01X",&ch);
			if (rc!=1)
				break;
			ss++;
			rc = sscanf(ss,"%01X",&cl);
			if (rc!=1)
				cl=0;
			ss++;
			int c=(ch<<4) | cl;
			
			len++;
			ns = realloc(ns,len);
			ns[len-1]=c;


		}while (rc==1);


		return bstr_create(ns,len);


	}

	return NULL;
}


