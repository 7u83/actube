#include "cw.h"
#include "capwap.h"
#include "log.h"
#include "dbg.h"
#include "capwap_items.h"

int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data, int len)
{
	int i = 0;
	while (i < len) {

		if (i + 8 > len) {
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
			cw_dbg(DBG_ELEM_ERR,
			       "WTP Descriptor sub-element too long, length = %d",
			       sublen);
			return 0;
		}

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

		switch (subtype) {
			case CW_SUBELEM_WTP_HARDWARE_VERSION:
				mbag_set_bstrv(mbag,
						   CW_ITEM_WTP_HARDWARE_VERSION,
						   vendor_id, data + i, sublen);

				break;
			case CW_SUBELEM_WTP_SOFTWARE_VERSION:

				mbag_set_bstrv(mbag,
						   CW_ITEM_WTP_SOFTWARE_VERSION,
						   vendor_id, data + i, sublen);
				break;
			case CW_SUBELEM_WTP_BOOTLOADER_VERSION:

				mbag_set_bstrv(mbag,
						   CW_ITEM_WTP_BOOTLOADER_VERSION,
						   vendor_id, data + i, sublen);

				break;
			default:
				cw_dbg(DBG_ELEM_ERR,
				       "Unknown WTP descriptor subelement, type = %d",
				       subtype);
				break;
		}
		i += sublen;

	}
	return 1;
}
