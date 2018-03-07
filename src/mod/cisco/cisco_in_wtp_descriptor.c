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

#include "cw/keys.h"
#include "cw/cw.h"
#include "cw/vendors.h"
#include "cw/capwap_items.h"


int cisco_in_wtp_descriptor(struct conn *conn, struct cw_ElemHandler *eh, uint8_t * data,
			 int len, struct sockaddr *from)
{

	static struct cw_DescriptorSubelemDef allowed[] = {
		{CW_VENDOR_ID_CISCO,CW_SUBELEM_WTP_HARDWARE_VERSION, CW_KEY_HARDWARE, 1024,0},
		{CW_VENDOR_ID_CISCO,CW_SUBELEM_WTP_SOFTWARE_VERSION, CW_KEY_SOFTWARE, 1024.0},
		{CW_VENDOR_ID_CISCO,CW_SUBELEM_WTP_BOOTLOADER_VERSION, CW_KEY_BOOTLOADER, 1024.0},
		{CW_VENDOR_ID_CISCO,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, CW_KEY_OTHER_SOFTWARE, 1024.0},
		{0,0, NULL, 0,0}
	};


	return cw_read_wtp_descriptor_7(conn->remote_cfg, conn, eh, data, len, allowed);
}



