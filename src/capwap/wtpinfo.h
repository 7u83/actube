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

#ifndef __WTPINFO_H
#define __WTPINFO_H

#include <stdint.h>
#include <sys/socket.h>

#include "radioinfo.h"

/* structure to hold info about a wtp */
struct wtpinfo{
	uint8_t *name;
	uint8_t * location;

	uint32_t vendor_id;
	uint8_t* model_no;
	uint8_t* serial_no;

	int max_radios;
	int radios_in_use;
	uint8_t frame_tunnel_mode;

	uint8_t mac_type;
	uint8_t * session_id;
	int session_id_len;

	struct radioinfo radioinfo[31];


	uint8_t * hardware_version;
	uint32_t hardware_vendor_id;
	uint8_t * software_version;
	uint32_t  software_vendor_id;
	uint8_t * bootloader_version;
	uint32_t bootloader_vendor_id;

	uint8_t * macaddress;
	int macaddress_len;
	uint8_t ecn_support;

	uint16_t max_msg_len;
	uint8_t discovery_type;

	struct sockaddr_storage local_ip;


};

extern int wtpinfo_readelem_wtp_board_data(struct wtpinfo *wtpinfo, int type, uint8_t *msgelem, int len);
extern int wtpinfo_readelem_wtp_mac_type(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_wtp_radio_info(struct wtpinfo * wtpinfo,int type,uint8_t *msgelem, int len);
extern int wtpinfo_readelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len);
extern int wtpinfo_readelem_wtp_frame_tunnel_mode(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_discovery_type(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_location_data(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_wtp_name(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_session_id(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
//extern int wtpinfo_readelem_ecn_support(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);
extern int wtpinfo_readelem_cw_local_ip_addr(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len);

#define wtpinfo_readelem_ecn_support(wtpinfo,type,msg,len) cw_readelem_ecn_support(&wtpinfo->ecn_support,type,msg,len)

#endif
