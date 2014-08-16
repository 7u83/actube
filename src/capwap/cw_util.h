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

#include <stdint.h>
#include <time.h>

extern uint8_t * cw_setstr(uint8_t ** dst, const uint8_t *src, int len);


extern int cw_foreach_msgelem(uint8_t * msgelems,  int len,
		int (*callback)(void*,int,uint8_t*,int),void *arg );


extern int cw_pseudo_rand(uint8_t *dst, int len);
extern int cw_rand(uint8_t*dst, int len);

extern int cw_is_printable(const uint8_t * s,int len);


#define cw_timer_start(t) (time(NULL)+t)
#define cw_timer_timeout(t) (time(NULL)>t ? 1 : 0)
