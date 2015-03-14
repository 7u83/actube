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

#ifndef __BSTR_H
#define __BSTR_H


#include <stdint.h>

typedef uint8_t* bstr_t;

extern uint8_t * bstr_create(uint8_t *data, uint8_t len);
extern uint8_t * bstr_create_from_cfgstr(const char * s);
extern uint8_t * bstr_replace( uint8_t ** dst, uint8_t * bstr);

extern int bstr_to_str(char *dst, bstr_t str,char * def);


#define bstr_len(s) (*(s))
#define bstr_data(s) (s+1)
#define bstr_size(len) (len+1)

#define BSTR_MAX_LEN 254


#endif

