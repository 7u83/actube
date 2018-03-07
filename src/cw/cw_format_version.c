#include <stdio.h>
#include "format.h"

/**
 * @brief Format a version element
 * @param s
 * @param version
 * @param len
 * @return number of bytes written
 */
int cw_format_version(char *s, const uint8_t * version, int len)
{
	int dot,rs,i;;

	rs = 0;

	if (format_is_utf8(version, len)) {
		if (len != 0)
			rs += sprintf(s + rs, "%.*s", len, version);
		else
			rs += sprintf(s + rs, "''");
	} else {
		for (i = 0; i < len && i < 20; i++) {
			rs += sprintf(s + rs, "%02X", version[i]);
		}

		dot = 0;

		rs += sprintf(s + rs, " (");
		for (i = 0; i < len && i < 20; i++) {
			if (dot)
				rs += sprintf(s + rs, ".");
			dot = 1;
			rs += sprintf(s + rs, "%d", version[i]);
		}
		rs += sprintf(s + rs, ")");
	}
/*
	vendor = bstrv_get_vendor_id(ver);
	rs += sprintf(s + rs, ", Vendor Id: %d, %s", vendor, cw_strvendor(vendor));
*/
	return rs;
}

