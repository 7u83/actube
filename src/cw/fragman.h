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
 *@defgroup Fragman FRAGMAN
 *@brief Frgaman functions
 *
 * Detailed desription
 *@{
 */


#ifndef __FRAGMAN_H
#define __FRAGMAN_H

#include <stdint.h>
#include <time.h>
#include <unistd.h>


#define MAX_PKT_HDR_LEN 64

/** maximaum size of a fragment */
#define FRAG_MAXSIZE (65536+MAX_PKT_HDR_LEN)

#define FRAG_MAXIDS 10

/**
 * TTL of received fragment in seconds
 */
#define FRAG_TTL 5 


struct frag {
	/*
//	uint8_t buffer[FRAG_MAXSIZE];
	
	 */
	uint8_t * buffer;
	int fragid;
	int bytesreceived;
	int bytesneeded;
	struct timespec t;
//	uint8_t * header;
};

typedef struct frag frag_t; /*FRAGMAN;*/

/*
//extern struct frag * fragman_add(struct frag * frags[], uint8_t *packet, int len); // struct cw_transport_header * th)
//extern uint8_t * fragman_add(struct frag * frags, uint8_t *packet, int len); // struct cw_transport_header * th)
*/
extern uint8_t * fragman_add( frag_t * frags, uint8_t *packet, int hlen, int payloadlen );

extern frag_t * fragman_create();
extern void fragman_destroy(frag_t * frags);

/*
//extern frag_init(struct frag ** frag[]);
*/

extern void fragman_free(frag_t * frags,struct frag * f);


/**@}*/

#endif

