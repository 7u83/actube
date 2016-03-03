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

#include <stdlib.h>
#include <stdio.h>

#include "file.h"

/**
 * @file
 */

/**
 * Load a file from disk to memory
 * @param filename name of file to load
 * @param size variable which receives the size of the file in bytes
 * @return a pointer to the memory where the file was loaded in \n 
 * The memory allocated returned in data must be freed using free.
 *
 * Eexample:
 * \code
   #include "capwap/file.h"
   size_t bytes;
   char * data = cw_load_file("file.txt",&bytes);
   if (data){
	// Do something with data ...
	free (data);
   }
   \endcode
 */
char *cw_load_file(const char *filename, size_t * size)
{
	FILE *infile = fopen(filename, "rb");
	if (!infile)
		return NULL;
	fseek(infile, 0, SEEK_END);
	*size = ftell(infile);
	char *buf = malloc(*size);
	if (!buf)
		goto errX;

	fseek(infile, 0, SEEK_SET);
	*size = fread(buf, 1, *size, infile);
      errX:
	fclose(infile);
	return buf;
}
