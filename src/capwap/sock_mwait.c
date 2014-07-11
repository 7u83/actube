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
#include <sys/select.h>
#include <stdlib.h>
#include <errno.h>

#include "sock.h"

int sock_mwait(int * socklist, int socklistlen,fd_set * fset)
{
	int i;

	FD_ZERO(fset);

	int max = 0;
	for (i=0; i<socklistlen; i++){
		if (socklist[i]<=0)
			continue;
		FD_SET(socklist[i],fset);
		if (socklist[i]>max)
			max=socklist[i];
	}

	int n;
        while((n=select(max+1, fset, NULL, NULL, NULL)) < 0) {
		if (errno != EINTR) 
			return n;
        }
	return n;
}

