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
#include "cwmsg.h"

void cwmsg_addelem_image_identifier(struct cwmsg *cwmsg,uint32_t vendor_id,uint8_t *img, int len)
{
//	uint32_t net_vendor_id = htonl(vendor_id);

	cwmsg_vaddelem(cwmsg,CWMSGELEM_IMAGE_IDENTIFIER,1,img,len);
//	cwmsg_vaddelem(cwmsg,CWMSGELEM_IMAGE_IDENTIFIER,2,&net_vendor_id,sizeof(net_vendor_id),img,len);
}

