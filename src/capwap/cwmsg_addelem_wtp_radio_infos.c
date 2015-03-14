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
#include "capwap_ieee80211.h"
#include "radioinfo.h"

void cwmsg_addelem_wtp_radio_info(struct cwmsg * msg,struct radioinfo *radioinfo)
{

	uint8_t ri[5];
	*ri = radioinfo->rid;

	*((uint32_t*)(ri+1))=htonl(radioinfo->type);
	cwmsg_addelem(msg,CWMSGELEM_IEEE80211_WTP_RADIO_INFO,ri,5);
}



void cwmsg_addelem_wtp_radio_infos(struct cwmsg * msg,struct radioinfo * radioinfos)
{
	int i;
	for (i=0; i<2; i++)
	{
//		if (radioinfos[i].rid!=0)	
			cwmsg_addelem_wtp_radio_info(msg,&radioinfos[i]);
	}

}

