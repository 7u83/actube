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
 *@file
 *@brief Definitions for imisc. cw_utill functions.
 */

#ifndef __CW_UTIL_H
#define __CW_UTIL_H

#include <stdint.h>
#include <time.h>

#include "bstr.h"

extern int cw_format_version(char *s, bstr_t version, uint32_t vendor, char * def);
extern int cw_is_printable(const uint8_t * s,int len);





extern uint8_t * cw_setstr(uint8_t ** dst, const uint8_t *src, int len);

extern int cw_foreach_msgelem(uint8_t * msgelems,  int len,
		int (*callback)(void*,int,uint8_t*,int),void *arg );





//#define cw_timer_start(t) (time(NULL)+t)
//#define cw_timer_timeout(t) (time(NULL)>t ? 1 : 0)


/** Generic macro to isolate bits from a dword */
#define cw_get_dword_bits(src,start,len) ((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))


/* functions used to "rfc-verify" message elements */
//void cw_mand_elem_found(int *l,int type);
//int cw_is_missing_mand_elems(int *l);
//void cw_get_missing_mand_elems(char *dst, int *l);



//extern const char * cw_msgtostr(int type);

#endif
