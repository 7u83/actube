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
 * @addtogroup Fragman
 * @{
 */

/**
 * @file
 * @brief This file implements a simple fragment management system for the capwap protocol.
 *
 * Usage goes as follows.
 *
 * First create a frag_t object:
 *
 * frag_t * fm = fragman_create() 
 *
 * Now you can add received fragmented packets to the fragment manager:
 *
 * fragman_add(fm,packet,len)
 *
 * if you have got all fragments fragman_add will return the the 
 * reasembled packet in a struct frag. if not all packets are recieved
 * fragman_add will return NULL. To check if there was an error you have 
 * to consult errno. errno will be set if fragman_add returns NULL and there
 * was another problem storing the received packet.
 *
 * After you have got a reassembled packet and processed it yout must 
 * free the memory by calling fragman_free()
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include "capwap.h"
#include "fragman.h"

/**
 * check if we keep already fragments belonging to the 
 * specified fragment id
 */
static struct frag * frag_get(struct frag * frags, int fragid)
{
	int i;
	for (i=0;i<FRAG_MAXIDS; i++){
		if (frags[i].buffer==NULL)
			continue;
		if (frags[i].fragid==fragid)
		{
			return &frags[i];
		}
	}
	return NULL;
}

/**
 * create a new fragment
 */
static struct frag * frag_new(struct frag * frags, int fragid)
{
        struct timespec t;
	clock_gettime(CLOCK_REALTIME,&t);

	int i;
	for (i=0; i<FRAG_MAXIDS; i++){
		if (frags[i].buffer==NULL || (t.tv_sec - frags[i].t.tv_sec>FRAG_TTL) ){
		//	printf("******************************new frag, %p\n",frags[i].buffer);

			frags[i].buffer=realloc(frags[i].buffer,FRAG_MAXSIZE);
			if (frags[i].buffer==NULL)
				return NULL;
			frags[i].t=t;
			frags[i].fragid=fragid;
			frags[i].bytesreceived=0;
			frags[i].bytesneeded=0;
			return &frags[i];
		}
	}
	return NULL;
}

/*
void fragman_free(frag_t * frags,struct frag * f)
{
	int i;
	for (i=0; i<FRAG_MAXIDS; i++){
		if (frags[i]->fragid==f->fragid){
			frags[i]=NULL;
			break;
		}
	}
	free(f);
}
*/

/**
 * Add a fragment
 * @pram frags Fragman object
 * @param packet Pointer to data of the packet to add
 * @param hlen Length of the header od the packet
 * @param payloadlen Number of bytes in payload
 * @return NULL if someting went wrong
 */
uint8_t * fragman_add(frag_t * frags, uint8_t *packet, int hlen, int payloadlen) 
{
	struct frag * f;
	uint32_t val0,val1;

	/* read the transport header dword 0,
	 * contains hlen*/	
	val0 = ntohl(*((uint32_t*)packet));
//	int hlen = (val0 >> 19) & 0x1f;

	/* read the transport header dword 1,
	 * contains fragid and fragoffset */	
	val1 = ntohl(*(((uint32_t*)packet)+1));
	int fragid = val1>>16;
	int fragoffset=(val1 >>3) & 0x1fff;

//	printf("Fragid = %i, offset = %i\n",fragid,fragoffset);
	
	/* determine size of payload */
/*	int payloadlen = len - hlen*4;
	if (payloadlen<0){
		errno = EINVAL;
		return NULL;
	}
*/
	/* find / create cfragment */
	f = frag_get(frags,fragid);
	if (!f){
		f = frag_new(frags,fragid);
	}
	if (!f){
		errno = ENOMEM;
		/* out of fragmentation space */
		return NULL;
	}

	errno = 0;

	int dst = fragoffset*8;

	/* copy fragment*/
	if (dst + payloadlen < FRAG_MAXSIZE) {
		memcpy( f->buffer+4+dst,packet+hlen,payloadlen);
		f->bytesreceived+=payloadlen;
	}

	if (val0  & CW_FLAG_HDR_L){
		f->bytesneeded=dst+payloadlen;
	}

	int ti;
	for (ti=0; ti<16; ti++){
//		printf("%02X ",(f->buffer+4)[ti]);

	}


	if (f->bytesneeded>0 && f->bytesneeded<=f->bytesreceived){
		uint8_t * r=f->buffer;
		f->buffer=0;
//		printf("last bytes need %i\n",f->bytesneeded);
		*((uint32_t*)(r))=f->bytesneeded;
		return r; 
	}

//	printf("Fragman bytes needed: %i, received  %i\n",f->bytesneeded,f->bytesreceived);
	return NULL;
}

frag_t * fragman_create() 
{
	frag_t * frags;
	frags = malloc(sizeof(struct frag)*FRAG_MAXIDS);
	if (frags == NULL )
		return NULL;
	memset (frags,0,(sizeof (struct frag ))*FRAG_MAXIDS);
	return frags;
}

void fragman_destroy(frag_t * frags)
{
	int i;
	for (i=0;i<FRAG_MAXIDS; i++){
		if (frags[i].buffer==NULL)
			continue;
		free(frags[i].buffer);
	}
	free (frags);	
}


/**
 * @}
 */

