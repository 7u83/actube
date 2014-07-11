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

#include <stdlib.h>
#include <string.h>

#include "cw_util.h"

uint8_t * cw_setstr(uint8_t ** dst, const uint8_t *src, int len)
{
	*dst = realloc(*dst,len+1);
	if (*dst==NULL)
		return NULL;
	memcpy(*dst,src,len);
	(*dst)[len]=0;
	return *dst;
}


