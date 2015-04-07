#ifndef __CAPWAP_CRYPTO_H
#define __CAPWAP_CRYPTO_H

#include <nettle/md5.h>
/**
 *@file
 *@brief
 */

/** MD5 digest size, 16 bytes */
#define CW_MD5_DIGEST_SIZE MD5_DIGEST_SIZE


int cw_fgetmd5sum(uint8_t *digest, FILE *infile);


#endif
