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
#include "bstr.h"


struct wtp_reboot_statistics{
	int reboot_count;
	int ac_initiated_count;
	int link_failure_count;
	int sw_failure_count;
	int hw_failure_count;
	int other_failure_count;
	int unknown_failure_count;
	int last_failure_type;
};


/* structure to hold info about a wtp */
struct wtpinfo{
	


	uint8_t *ac_name;
	uint8_t *name;
	uint8_t * location;

	uint32_t vendor_id;
	uint8_t* model_no;
	uint8_t* serial_no;

	bstr_t board_id;
	bstr_t board_revision;

	uint16_t statistics_timer;

	int max_radios;
	int radios_in_use;
	uint8_t frame_tunnel_mode;

	uint8_t mac_type;

	
	bstr_t session_id;
//	int session_id_len;

	struct radioinfo radioinfo[31];


	uint8_t * hardware_version;
	int hardware_version_len;
	uint32_t hardware_vendor_id;
	uint8_t * software_version;
	int software_version_len;
	uint32_t  software_vendor_id;
	uint8_t * bootloader_version;
	int bootloader_version_len;
	uint32_t bootloader_vendor_id;


	uint16_t encryption_cap;	
	
	

	uint8_t * macaddress;
	int macaddress_len;
	uint8_t ecn_support;

	uint16_t max_msg_len;
	uint8_t discovery_type;

	struct sockaddr_storage local_ip;
	struct wtp_reboot_statistics reboot_statistics;

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
extern int wtpinfo_print(char *str, struct wtpinfo * wtpinfo);

#define wtpinfo_readelem_ecn_support(wtpinfo,type,msg,len) cw_readelem_ecn_support(&wtpinfo->ecn_support,type,msg,len)



extern int wtpinfo_lwreadelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len);

#define wtpinfo_readelem_ac_name(wtpinfo,type,msg,len) cw_readelem_ac_name(&wtpinfo->ac_name,type,msg,len)


#endif
