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

#include "capwap.h"
#include "dbg.h"
#include "sock.h"

#include "log.h"

/**
 * Put a list of missing mandatory message elements to debug output
 */
void cw_dbg_missing_mand(int level,struct conn *conn,cw_action_in_t ** ml,int n,cw_action_in_t *a)
{
	if (!cw_dbg_is_level(DBG_ELEM) || n==0)
		return;

	char buffer[2000];
	char *p=buffer;
	int i;
	char *delim="";
	for(i=0; i<n; i++) {
		p+=sprintf(p,"%s",delim); delim=", ";
		p+=sprintf(p,"%s",cw_strelemp(conn->actions,ml[i]->elem_id));
	}
	cw_dbg(DBG_ELEM,"Missing mandatory elements: [%s]",buffer);
}



void cw_dbg_packet(struct conn *conn, uint8_t * packet, int len)
{
	if (!cw_dbg_is_level(DBG_CW_PKT_IN || DBG_CW_PKT_OUT))
		return;


	/* print the header */
	char hdr[200];
	hdr_print(hdr, packet, len);


	if (!cw_dbg_is_level(DBG_CW_PKT_DMP)) {
		cw_dbg(DBG_CW_PKT_IN, "Processing capwap packet from %s, len=%d\n%s",
		       sock_addr2str(&conn->addr), len, hdr);
		return;

	}

	cw_dbg_dmp(DBG_CW_PKT_DMP, packet, len,
		   "Processing packet from %s, len=%d\n%s\n\tDump:",
		   sock_addr2str(&conn->addr), len, hdr);

}


void cw_log_dbg_dmp_(int level, const char *file, int line,
		     const uint8_t * data, int len, const char *format, ...)
{

	if (!(level & cw_dbg_opt_level))
		return;


	int maxtlen = 2048;
	int i;
	int rowlen = CW_LOG_DUMP_ROW_LEN;
	int rows = len / rowlen;
	int tlen = 0;

	int md;
	if (cw_dbg_opt_detail & DBG_DETAIL_ASC_DMP)
		md = 2;
	else
		md = 1;



	char *dst = malloc(md * (len * 3 + (rows * 2) + 8 + maxtlen));
	if (!dst)
		return;

	if (format != NULL) {
		va_list args;
		va_start(args, format);
		tlen = vsnprintf(dst, maxtlen, format, args);
		va_end(args);
	}



	if (len % CW_LOG_DUMP_ROW_LEN)
		rows++;



	char *pdst = dst + tlen;
	sprintf(pdst, "\n\t");
	pdst += 2;

	char asc_buffer[128];
	char *ascdst = asc_buffer;

	for (i = 0; i < len; i++) {
		sprintf(pdst, "%02X ", data[i] & 0xff);

		if (cw_dbg_opt_detail & DBG_DETAIL_ASC_DMP) {
			int c = data[i] & 0xff;
			if (c < 0x20 || c > 0x80)
				c = '.';
			*ascdst = c;
			ascdst++;
		}

		pdst += 3;
		if ((i + 1) % rowlen == 0) {
			int l;
			if (cw_dbg_opt_detail & DBG_DETAIL_ASC_DMP) {
				*ascdst = 0;
				l = sprintf(pdst, " | %s\n\t", asc_buffer);
				ascdst = asc_buffer;

			} else {
				l = sprintf(pdst, "\n\t");
			}
			pdst += l;
		}

	}

	if (cw_dbg_opt_detail & DBG_DETAIL_ASC_DMP) {
		*ascdst = 0;
		if (strlen(asc_buffer))
			pdst += sprintf(pdst, " | %s", asc_buffer);

	}

	if (cw_dbg_opt_detail & DBG_DETAIL_LINE_NUMBERS)
		cw_log(LOG_DEBUG, "%s:%d: %s", file, line, dst);
	else
		cw_log(LOG_DEBUG, dst);

	free(dst);
	return;
}



