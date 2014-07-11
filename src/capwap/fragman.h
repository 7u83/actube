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


#ifndef __FRAGMAN_H
#define __FRAGMAN_H

#include <stdint.h>
#include <time.h>

#ifndef FRAG_MAXSIZE
	#define FRAG_MAXSIZE 65536+4
#endif

#ifndef FRAG_MAXIDS
	#define FRAG_MAXIDS 10
#endif

#ifndef FRAG_TTL
	#define FRAG_TTL 5
#endif


struct frag {
//	uint8_t buffer[FRAG_MAXSIZE];
	uint8_t * buffer;
	int fragid;
	int bytesreceived;
	int bytesneeded;
	struct timespec t;
};

typedef struct frag FRAGMAN;


//extern struct frag * fragman_add(struct frag * frags[], uint8_t *packet, int len); // struct cw_transport_header * th)
//extern uint8_t * fragman_add(struct frag * frags, uint8_t *packet, int len); // struct cw_transport_header * th)
extern uint8_t * fragman_add( FRAGMAN * frags, uint8_t *packet, int hlen, int payloadlen );

extern FRAGMAN * fragman_create();
extern void fragman_destroy(FRAGMAN * frags);


//extern frag_init(struct frag ** frag[]);
extern void fragman_free(FRAGMAN * frags,struct frag * f);


#endif

