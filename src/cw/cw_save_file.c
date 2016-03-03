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
    along with libcapwap.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 *@file
 */

#include <stdio.h>

#include "file.h"

/**
 * Save data from memory to a file
 * @param filename file to save data to
 * @data pointer to data to save
 * @len number of bytes to save
 * @return number of bytes saved
 *
 * Use ferror to determine if an error has occured, when 
 * the return value (number of bytes written) differs from len.
 */ 
int cw_save_file(const char *filename, char *data, int len)
{
	FILE *outfile = fopen(filename, "wb");
	if (!outfile)
		return 0;

	int bytes = fwrite(data, 1, len, outfile);
	fclose(outfile);
	return bytes;
}
