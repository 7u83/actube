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
#include "capwap_items.h"


#include "cw_util.h"
#include "dbg.h"

#include "sock.h"


static int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data,
					   int len, int silent)
{
	int i = 0;
	while (i<len) {

		if (i + 8 > len) {
			if (!silent)
				cw_dbg(DBG_ELEM_ERR,
				       "WTP descriptor subelement to long, length=%d>%d",
				       i + 8, len);
			return 0;
		}

		uint32_t vendor_id = cw_get_dword(data + i);
		uint32_t val = cw_get_dword(data + i + 4);

		int subtype = (val >> 16) & 0xffff;
		int sublen = val & 0xffff;
		i += 8;

		if (sublen + i > len) {
			if (!silent)
				cw_dbg(DBG_ELEM_ERR,
				       "WTP Descriptor sub-element too long, length = %d",
				       sublen);
			return 0;
		}

		if (!silent) {
			char *dmp;
			char *dmpmem = NULL;
			if (cw_dbg_is_level(DBG_SUBELEM_DMP)) {
				dmpmem = cw_dbg_mkdmp(data + i, sublen);
				dmp = dmpmem;
			} else
				dmp = "";
			cw_dbg(DBG_SUBELEM, "WTP Descriptor subtype=%d,len=%d%s", subtype,
			       sublen, dmp);

			if (dmpmem)
				free(dmpmem);
		}

		switch (subtype) {
			case CW_SUBELEM_WTP_HARDWARE_VERSION:
/*
				mbag_set_dword(mbag,
						       CW_ITEM_WTP_HARDWARE_VENDOR,
						       vendor_id);
				mbag_set_bstrn(mbag,
						       CW_ITEM_WTP_HARDWARE_VERSION,
						       data + i, sublen);
*/

				mbag_set_vendorstr(mbag,
							   CW_ITEM_WTP_HARDWARE_VERSION,
							   vendor_id, data + i, sublen);

				break;
			case CW_SUBELEM_WTP_SOFTWARE_VERSION:

				mbag_set_vendorstr(mbag,
							   CW_ITEM_WTP_SOFTWARE_VERSION,
							   vendor_id, data + i, sublen);
/*
				mbag_set_dword(mbag,
						       CW_ITEM_WTP_SOFTWARE_VENDOR,
						       vendor_id);
				mbag_set_bstrn(mbag,
						       CW_ITEM_WTP_SOFTWARE_VERSION,
						       data + i, sublen);

*/
				break;
			case CW_SUBELEM_WTP_BOOTLOADER_VERSION:

				mbag_set_vendorstr(mbag,
							   CW_ITEM_WTP_BOOTLOADER_VERSION,
							   vendor_id, data + i, sublen);

/*
				mbag_set_dword(mbag,
						       CW_ITEM_WTP_BOOTLOADER_VENDOR,
						       vendor_id);
				mbag_set_bstrn(mbag,
						       CW_ITEM_WTP_BOOTLOADER_VERSION,
						       data + i, sublen);
*/
				break;
			default:
				if (!silent)
					cw_dbg(DBG_ELEM_ERR,
					       "Unknown WTP descriptor subelement, type = %d",
					       subtype);
				break;
		}
		i += sublen;

	} //while (i < len);

	return 1;

}

static int cw_read_wtp_descriptor(mbag_t mbag, struct conn *conn,
				  struct cw_action_in *a, uint8_t * data, int len,
				  int silent)
{


	mbag_set_byte(mbag, CW_ITEM_WTP_MAX_RADIOS, cw_get_byte(data));
	mbag_set_byte(mbag, CW_ITEM_WTP_RADIOS_IN_USE,
			      cw_get_byte(data + 1));


	/* Get number of encryption elements */
	int ncrypt = cw_get_byte(data + 2);
	if (ncrypt == 0) {
		if (conn->strict_capwap) {
			if (!silent)
				cw_dbg(DBG_ELEM_ERR,
				       "Bad WTP Descriptor, number of encryption elements is 0.");
			return 0;
		}
		if (!silent)
			cw_dbg(DBG_RFC,
			       "Non standard conform WTP Descriptor, number of encryptoin elements is 0.");
	}

	int pos = 3;
	int i;
	for (i = 0; i < ncrypt; i++) {
		// It's a dummy for now
		pos += 3;
	}

	return cw_read_wtp_descriptor_versions(mbag, data + pos, len - pos, silent);

}



/**
 * Read WTP Descriptor in Cisco-Style (Draft 7)
 */

static int cw_read_cisco_wtp_descriptor(mbag_t mbag, struct conn *conn,
					struct cw_action_in *a, uint8_t * data, int len,
					int silent)
{

	mbag_set_byte(mbag, CW_ITEM_WTP_MAX_RADIOS, cw_get_byte(data));
	mbag_set_byte(mbag, CW_ITEM_WTP_RADIOS_IN_USE,
			      cw_get_byte(data + 1));


	int pos = 2;

	/* Encryption element, for now dumy XXX */
	//cw_get_word(data + pos + 2);
	pos += 2;

	return cw_read_wtp_descriptor_versions(mbag, data + pos, len - pos, silent);

}








int cw_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from)
{
	mbag_t mbag = conn->incomming;

	return cw_read_wtp_descriptor(mbag, conn, a, data, len, 0);
}



int cw_in_cipwap_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len, struct sockaddr *from)
{
	mbag_t mbag = conn->incomming;

	switch (conn->capwap_mode) {
		case CW_MODE_CISCO:
			return cw_read_cisco_wtp_descriptor(mbag, conn, a, data, len,
							    0);
		case CW_MODE_CIPWAP:
			{
				/* try to get the right WTP Descriptor */
				int rc;
				rc = cw_read_wtp_descriptor(mbag, conn, a, data, len,
							    1);

				if (rc) {
					return cw_read_wtp_descriptor(mbag, conn, a,
								      data, len, 0);
				}

				rc = cw_read_cisco_wtp_descriptor(mbag, conn, a,
								  data, len, 0);
				if (rc) {
					return cw_read_cisco_wtp_descriptor(mbag,
									    conn, a, data,
									    len, 0);
				}

				return cw_read_wtp_descriptor(mbag, conn, a, data,
							      len, 0);

			}
		default:
			return cw_read_wtp_descriptor(mbag, conn, a, data, len, 0);


	}

	return 0;
}
