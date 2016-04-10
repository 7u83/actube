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

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "sock.h"
#include "netconn.h"
#include "log.h"
#include "dbg.h"
#include "capwap.h"
#include "cw.h"

static int netconn_recv_packet_(struct netconn *nc, uint8_t * buf, int len, int flags)
{
	int n;
	while ((n = recv(nc->sock, (char *) buf, len, flags)) < 0) {
		if (errno != EINTR) {
			if (errno == EAGAIN)
				return n;

		}
	}
	return n;
}

int netconn_recv_packet(struct netconn *nc, uint8_t * buf, int len)
{
	return netconn_recv_packet_(nc, buf, len, 0);
}

int netconn_recv_packet_peek(struct netconn *nc, uint8_t * buf, int len)
{
	int rc = netconn_recv_packet_(nc, buf, len, MSG_PEEK);
	return rc;

}

static uint8_t *netconn_q_get_packet(struct netconn *nc)
{
	struct timespec timespec;
	clock_gettime(CLOCK_REALTIME, &timespec);
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;
	timespec.tv_sec++;

	/* wait one second to get a packet */
	if (sem_timedwait(&nc->q_sem, &timespec) == -1) {
		return NULL;
	};

	int qrpos = nc->qrpos + 1;
	if (qrpos == nc->qsize)
		qrpos = 0;
	nc->qrpos = qrpos;
	return nc->q[qrpos];
}

static int netconn_q_recv_packet_(struct netconn *nc, uint8_t * buffer, int len, int peek)
{
	if (!nc->cur_packet) {
		if ((nc->cur_packet = netconn_q_get_packet(nc)) == 0) {
			errno = EAGAIN;
			return -1;
		}
		nc->cur_packet_len = *((uint32_t *) nc->cur_packet);
		nc->cur_packet_pos = 4;
	}

	if (nc->cur_packet_len > len) {
		memcpy(buffer, nc->cur_packet + nc->cur_packet_pos, len);
		if (peek)
			return len;

		nc->cur_packet_pos += len;
		nc->cur_packet_len -= len;
		if (nc->cur_packet_len == 0) {
			free(nc->cur_packet);
			nc->cur_packet = 0;
		}
		return len;
	}

	memcpy(buffer, nc->cur_packet + nc->cur_packet_pos, nc->cur_packet_len);
	if (peek)
		return nc->cur_packet_len;

	free(nc->cur_packet);
	nc->cur_packet = 0;
	return nc->cur_packet_len;
}

int netconn_q_recv_packet(struct netconn *nc, uint8_t * buffer, int len)
{
	return netconn_q_recv_packet_(nc, buffer, len, 0);
}

int netconn_q_recv_packet_peek(struct netconn *nc, uint8_t * buffer, int len)
{
	return netconn_q_recv_packet_(nc, buffer, len, 1);
}

void netconn_q_add_packet(struct netconn * nc,uint8_t *packet,int len)
{
	int qwpos = nc->qwpos;
	if (qwpos==nc->qsize)
		qwpos=0;

	if (nc->qrpos==qwpos){
		/* no buffers, discard packet */
		cw_dbg(DBG_PKT_ERR, "Discarding packet from %s, no queue buffers left",
			sock_addr2str(&nc->addr));
		return;
	}

	nc->q[qwpos]=malloc(len+4);
	if (nc->q[qwpos]==NULL)
		return;
	
	*((uint32_t*)(nc->q[qwpos]))=len;
	memcpy(nc->q[qwpos]+4,packet,len);	
	nc->qwpos=qwpos+1;

	sem_post(&nc->q_sem);
}






void netconn_destroy(struct netconn *nc)
{
	if (!nc)
		return;

	if (nc->fragman)
		fragman_destroy(nc->fragman);

	if (nc->q)
		free(nc->q);

	free(nc);
}


int netconn_send_packet(struct netconn *nc, const uint8_t * buffer, int len)
{
	int n;
	while ((n = sendto(nc->sock, buffer, len, 0,
			   (struct sockaddr *) &nc->addr,
			   sock_addrlen((struct sockaddr *) &nc->addr))) < 0) {

		if (errno == EINTR)
			continue;


		return n;
	}

	return n;
}




int netconn_send_capwap_msg(struct netconn * nc, uint8_t *rawmsg, int msglen)
{

//	int msglen = cw_get_hdr_msg_total_len(rawmsg);


	uint8_t * ptr = rawmsg;

	int fragoffset = 0;

	int hlen = cw_get_hdr_hlen(rawmsg)*4;

//	int mtu = nc->mtu;

	int mtu = 1400;

	while (msglen>mtu){
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F,1);
		cw_put_dword(ptr+4, nc->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt_nc(DBG_PKT_OUT,nc,ptr,mtu,(struct sockaddr*)&nc->addr);

		if (nc->write(nc,ptr,mtu)<0)
			return -1;

		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		msglen-=mtu-hlen;

	}


	if (fragoffset)
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F | CW_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(rawmsg,CW_FLAG_HDR_F,0);

	cw_put_dword(ptr+4, nc->fragid<<16 | fragoffset<<3 );

	cw_dbg_pkt_nc(DBG_PKT_OUT,nc,ptr,msglen,(struct sockaddr*)&nc->addr);

	return nc->write(nc,ptr,msglen-0);
}



int netconn_process_packet(struct netconn *nc, uint8_t * packet, int len,
			struct sockaddr *from)
{

	cw_dbg_pkt_nc(DBG_PKT_IN, nc, packet, len, from);
	if (len < 8) {
		/* packet too short */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, packet too short, len=%d,  at least 8 expected.",
		       sock_addr2str(&nc->addr), len);
		errno = EAGAIN;
		return -1;
	}

	int preamble = cw_get_hdr_preamble(packet);

	if ((preamble & 0xf0) != (CAPWAP_VERSION << 4)) {
		/* wrong version */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, wrong version, version=%d, version %d expected.",
		       sock_addr2str(&nc->addr), (preamble & 0xf0) >> 4,
		       CAPWAP_VERSION);
		errno = EAGAIN;
		return -1;
	}

	if (preamble & 0xf) {
		/* Encrypted data, this shuold never happen here */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, encrypted data after decryption ...",
		       sock_addr2str(&nc->addr));
		errno = EAGAIN;
		return -1;
	}


	int offs = cw_get_hdr_msg_offset(packet);


	int payloadlen = len - offs;
	if (payloadlen < 0) {
		/* Eleminate messages with wrong header size */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, header length (%d) greater than packet len (%d).",
		       sock_addr2str(&nc->addr), offs, len);
		errno = EAGAIN;
		return -1;
	}

	/* Check if Radio MAC is present */
	if (cw_get_hdr_flag_m(packet)) {

		if (cw_get_hdr_rmac_len(packet) + 8 > offs) {
			/* wrong rmac size */
			cw_dbg(DBG_PKT_ERR,
			       "Discarding packet from %s, wrong R-MAC size, size=%d",
			       sock_addr2str(&nc->addr), *(packet + 8));
			errno = EAGAIN;
			return -1;
		}

	}


	if (cw_get_hdr_flag_f(packet)) {
		/* fragmented, add the packet to fragman */
		uint8_t *f;
		f = fragman_add(nc->fragman, packet, offs, payloadlen);
		if (f == NULL) {
			errno = EAGAIN;
			return -1;
		}


		cw_dbg_pkt_nc(DBG_PKT_IN, nc, f + 4, *(uint32_t *) f, from);

		// XXX: Modify fragman to not throw away CAPWAP headers

		int rc = nc->process_message(nc, f + 4, *(uint32_t *) f, from);

		free(f);
		return rc;
	}

	/* not fragmented, we have a complete message */
	
	return nc->process_message(nc, packet, len, from);
}



/**
 * Used as main message loop
 */
int netconn_read_messages(struct netconn *nc)
{
	uint8_t buf[2024];
	int len = 2024;

	int n = nc->read(nc, buf, len);
	if (n < 0)
		return n;

	if (n > 0) {
		return nc->process_packet(nc, buf, n,
					    (struct sockaddr *) &nc->addr);
	}
	errno = EAGAIN;
	return -1;
}




/**
 * Create a netconn object
 */
struct netconn *netconn_create(int sock, struct sockaddr *addr, int qsize)
{
	struct netconn *nc = malloc(sizeof(struct netconn));
	if (!nc)
		return NULL;

	memset(nc, 0, sizeof(struct netconn));

	nc->sock = sock;
	sock_copyaddr(&nc->addr, addr);

	if (!qsize) {
		nc->recv_packet = netconn_recv_packet;
		nc->recv_packet_peek = netconn_recv_packet_peek;
		goto finalize;
	}


	if (!(nc->q = malloc(sizeof(uint8_t *) * qsize))) {
		netconn_destroy(nc);
		return NULL;
	}
	nc->qrpos = -1;
	if (sem_init(&nc->q_sem, 0, 0) != 0) {
		cw_log(LOG_ERR, "Fatal- Can't init semaphore for conn object: %s",
		       strerror(errno));
		netconn_destroy(nc);
		return NULL;
	};

	nc->recv_packet = netconn_q_recv_packet;
	nc->recv_packet_peek = netconn_q_recv_packet_peek;

      finalize:
	nc->send_packet = netconn_send_packet;
	nc->write = nc->send_packet;
	nc->read = nc->recv_packet;

	return nc;
}
