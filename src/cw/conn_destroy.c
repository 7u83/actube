/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 * @brief Implements conn_destroy
 */

#include <stdlib.h>
#include "conn.h"

/**
 * Destroy a conn object
 * @param object to destroy
 */
void conn_destroy(struct conn * conn)
{
	if (conn->fragman)
		fragman_destroy(conn->fragman);
	if (conn->q)
		free (conn->q);

	free(conn);
}


