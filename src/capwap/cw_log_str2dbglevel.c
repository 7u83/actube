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


#include <string.h>

#include "cw_log.h"


struct cw_dbg_cfgstrs
{
	const char *name;
	int level;
}; 

struct cw_dbg_cfgstrs cw_dbg_cfgstrs[] = {

	{"info",DBG_CW_INFO},
	{"msg",DBG_CW_MSG},
	{"msgelem",DBG_CW_MSGELEM},
	{"msgelem_dmp",DBG_CW_MSGELEM_DMP},
	{"rfc",DBG_CW_RFC},
	{"pkt_in",DBG_CW_PKT_IN},
	{"pkt_out",DBG_CW_PKT_OUT},
	{"pkt",(DBG_CW_PKT_IN | DBG_CW_PKT_OUT)},
	{"pkt_dmp",DBG_CW_PKT_DMP},
	{"pkt_err",DBG_CW_PKT_ERR},
	{"msg_err",DBG_CW_MSG_ERR},

	{"dtls",DBG_DTLS},
	{"dtls_dietail",DBG_DTLS_DETAIL},
	{"dtls_bio",DBG_DTLS_BIO},
	{"dtls_bio_dmp",DBG_DTLS_BIO_DMP},


	{"all",DBG_ALL},
	{"err",DBG_ERR},
	{0,0}

};

int cw_log_str2dbglevel(const char * str)
{
	int i;
	for (i=0; cw_dbg_cfgstrs[i].name; i++) {
		if (!strcmp(str,cw_dbg_cfgstrs[i].name))
			return cw_dbg_cfgstrs[i].level;
	}
	return 0;
}

