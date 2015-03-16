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
 * @brief: Generic handler for echo requests
 */

#include "capwap.h"

/**
 * Handles an echo request
 * @param d points to a #conn structure
 * @return 0=ok\nother Error code.
 *
 * This function is used as callback function in conjunction
 * with cw_wait_for_request
 */
int cw_handle_echo_request(void * d)
{
        struct conn * conn = (struct conn *)d;
        struct cwrmsg * cwrmsg = &conn->cwrmsg;
        cw_send_echo_response(conn,cwrmsg->seqnum,0);
        return 0;
}


