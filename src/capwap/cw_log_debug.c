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
 *@file
 *@brief Debug helpers
 */


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "cw_log.h"
#include "capwap.h"
#include "capwap_cisco.h"
#include "lwapp_cisco.h"

#include "cw_util.h"


int cw_dbg_opt_detail = 0;
int cw_dbg_opt_level = 0;



int cw_log_debug_level = 0;




static void cw_log_debug0_(const char *format, ...)
{
	if (cw_log_debug_level < 0)
		return;
	va_list args;
	va_start(args, format);
	cw_vlog_(LOG_DEBUG, format, args);
	va_end(args);
	closelog();
}

static void cw_log_debug1_(const char *format, ...)
{
	if (cw_log_debug_level < 1)
		return;

	va_list args;
	va_start(args, format);
	cw_vlog_(LOG_DEBUG, format, args);
	va_end(args);
	closelog();
}


static void cw_log_debug2_(const char *format, ...)
{
	if (cw_log_debug_level < 2)
		return;
	va_list args;
	va_start(args, format);
	cw_vlog_(LOG_DEBUG, format, args);
	va_end(args);
	closelog();
}




int cw_log_debug_dump_(int level, const uint8_t * data, int len, const char *format, ...)
{
	int maxtlen = 2048;
	int i;
	int rowlen = CW_LOG_DUMP_ROW_LEN;
	int rows = len / rowlen;
	int tlen = 0;

	char *dst = malloc(len * 3 + (rows * 2) + 8 + maxtlen);
	if (!dst)
		return 0;

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

	for (i = 0; i < len; i++) {
		sprintf(pdst, "%02X ", data[i] & 0xff);
		pdst += 3;
		if ((i + 1) % rowlen == 0) {
			sprintf(pdst, "\n\t");
			pdst += 2;
		}

	}

	cw_log_debug_cbs[level] ("%s", dst);

	free(dst);
	return 1;

}


void cw_log_dbg_(int level, const char *file, int line, const char *format, ...)
{

	if (!(level & cw_dbg_opt_level))
		return;

	char buf[2048];

	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	if (cw_dbg_opt_detail & DBG_DETAIL_LINE_NUMBERS)
		cw_log(LOG_DEBUG, "%s:%d: %s", file, line, buf);
	else
		cw_log(LOG_DEBUG, buf);
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






void (*cw_log_debug_cbs[]) (const char *fromat, ...) = {
cw_log_debug0_, cw_log_debug1_, cw_log_debug2_};








int cw_format_vendor(char *dst, uint32_t vendor_id, int elem_id, const uint8_t * elem_data)
{
	switch (vendor_id) {
		case CW_VENDOR_ID_CISCO:
			{
				sprintf(dst, "\n\t  Cisco Vendor Specific: %d - %s", elem_id,
					cw_cisco_id_to_str(elem_id));

				/* dive into LWAPP vendor specific decoding */
				if (elem_id == CW_CISCO_SPAM_VENDOR_SPECIFIC) {
					uint32_t lw_elem_id = lw_get_word(elem_data + 4 + 6);
					char b[256];
					sprintf(b, "\n\t   LWAPP Cisco Specific: %d - %s",
						lw_elem_id, lw_cisco_id_to_str(lw_elem_id));
					strcat(dst, b);



				}
				break;
			}



	}

	return 0;
}



/**
 * print debug info for message elements
 */

void cw_dbg_msgelem_(int msg, int msgelem, const uint8_t * msgbuf, int len)
{
	if (!cw_dbg_is_level(DBG_ELEM))
		return;

	const char *elemname;
	char vendorname[256];
	char vendor_details[265];
	*vendor_details = 0;

	if (msgelem == CW_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
		uint32_t vendor_id = ntohl(*((uint32_t *) msgbuf));
		int type = ntohs(*((uint16_t *) (msgbuf + 4)));
		sprintf(vendorname, "%s/%s/%d",
			(char *) cw_strelem(msgelem),
			(char *) lw_vendor_id_to_str(vendor_id), type);
		elemname = vendorname;
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);

	} else {
		elemname = cw_strelem(msgelem);
	}


	if (!cw_dbg_is_level(DBG_ELEM_DMP))
		cw_dbg(DBG_ELEM,
		       "%s, CAWPAP element: type=%d (%s), len=%d%s",
		       cw_strmsg(msg), msgelem, elemname, len, vendor_details);

	else
		cw_dbg_dmp(DBG_ELEM, msgbuf, len,
			   "%s, CAPWAP element: type=%d (%s), len=%d%s\n\tDump ...",
			   cw_strmsg(msg), msgelem, elemname, len, vendor_details);
}


void lw_dbg_elem_(int msg_id, int elem_id, const uint8_t * elem_data, int elem_len)
{
	if (!cw_dbg_is_level(DBG_ELEM))
		return;

	const char *elem_name;
	char vendorname[256];

	if (elem_id == LW_ELEM_VENDOR_SPECIFIC) {
		uint32_t vendor = lw_get_dword(elem_data);
		int type = lw_get_word(elem_data + 4);
		sprintf(vendorname, "%s/%s/%d",
			(char *) lw_elem_id_to_str(elem_id),
			(char *) lw_vendor_id_to_str(vendor), type);
		elem_name = vendorname;
	} else
		elem_name = lw_elem_id_to_str(msg_id);


	if (!cw_dbg_is_level(DBG_ELEM_DMP))
		cw_dbg(DBG_ELEM,
		       "%s, LWAPP element: type=%d (%s), len=%d",
		       lw_msg_id_to_str(msg_id), elem_id, elem_name, elem_len);

	else
		cw_dbg_dmp(DBG_ELEM, elem_data, elem_len,
			   "%s, LWAPP element: type=%d (%s), len=%d\n\tDump ...",
			   lw_msg_id_to_str(msg_id), elem_id, elem_name, elem_len);


}

/*
void cw_dbg_missing_mand_elems_(struct conn *conn, int msgtype, int *mand)
{
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return;

	if (cw_is_missing_mand_elems(mand)) {
		char str[512];
		cw_get_missing_mand_elems(str, mand);
		cw_dbg(DBG_CW_RFC, "Missing msgelems in %s: %s", cw_strmsg(msgtype), str);
	}
}
*/
