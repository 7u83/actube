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


