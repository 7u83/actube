
#include <stdio.h>
#include <errno.h>

#include <nettle/md5.h>

#include "capwap_crypto.h"

#define BLOCK_SIZE 4096

/**
 * Calculate MD5 checksum for an opened file. 
 * @param digest destination buffer for calculated 
 *   checksum (16 bytes, for a predifned constant use #CW_MD5_DIGEST_SIZE)
 * @param infile filehanle 
 * 
 * Remember to set the file pointer to the beginning of the file, before
 * calling this function. Therefor use fseek(file,0,SEEK_SET).
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

