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
 * @brief
 */ 

#include <stdio.h>
#include <errno.h>

#include <nettle/md5.h>

#include "capwap_crypto.h"

#define BLOCK_SIZE 4096

/**
 * Calculate MD5 checksum for an opened file. 
 * @param digest destination buffer for calculated 
 *   checksum (16 bytes, for a predifned constant use #CW_MD5_DIGEST_SIZE)
 * @param infile file hanle 
 * 
 * Remember to set the file pointer to the beginning of the file, before
 * calling this function. Therefore use fseek(file,0,SEEK_SET).
 */
int cw_fgetmd5sum(uint8_t *digest, FILE *infile)
{
	struct md5_ctx ctx;
	uint8_t buffer[BLOCK_SIZE];
	md5_init(&ctx);

	while (!feof(infile)){
		int bytes = fread(buffer,1,sizeof(buffer),infile);
		md5_update(&ctx,bytes,buffer);
	}		

	if (ferror(infile))
		return errno;
	
	md5_digest(&ctx, MD5_DIGEST_SIZE, digest);
	return 0;
}

