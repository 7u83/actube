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

#ifndef __DTLS_GNUTLS_H
#define __DTLS_GNUTLS_H

#include "conn.h"

extern int dtls_gnutls_init();
extern int dtls_gnutls_accept(struct conn * conn);
extern const char * dtls_gnutls_get_cipher(struct conn * conn);


#endif

