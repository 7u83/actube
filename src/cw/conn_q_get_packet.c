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

#include <time.h>
#include "conn.h"

uint8_t * conn_q_get_packet(struct conn * conn)
{
	struct timespec timespec;
	clock_gettime(CLOCK_REALTIME,&timespec);
	timespec.tv_sec++;

	/* wait one second to get a packet */
	if (sem_timedwait(&conn->q_sem,&timespec)==-1){
		return NULL;
	};

	int qrpos = conn->qrpos+1;
	if (qrpos==conn->qsize)
		qrpos=0;
	conn->qrpos=qrpos;
	return conn->q[qrpos];
}
