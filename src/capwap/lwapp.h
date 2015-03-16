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

#ifndef __LWAPP_H
#define __LWAPP_H

#include <stdint.h>
#include <arpa/inet.h>

#include "wtpinfo.h"


/* version */
#define LW_VERSION 0


/* ports */
#define LWAPP_CONTROL_PORT 12223
#define LWAPP_CONTROL_PORT_STR "12223"


/* macros to access transport header */
#define LWTH_GET_VERSION(th) (th[0]>>6)
#define LWTH_GET_L_FLAG(th) (th[0]&0x1)
#define LWTH_GET_F_FLAG(th) (th[0]&0x2)
#define LWTH_GET_C_FLAG(th) (th[0]&0x4)
#define LWTH_GET_RID(th) ((th[0]&0x38)>>3)
#define LWTH_GET_FRAGID(th) (th[1])
#define LWTH_GET_LENGTH(th) (ntohl(*((uint32_t*)(th)))&0xffff)

#define LWTH_SET_VERSION(th,v) (th[0] = (th[0]&0x3f) | (v<<6))
#define LWTH_SET_C_FLAG(th,v)  (th[0] = (th[0]&0xfB) | (v<<2))


#define LWMSG_GET_TYPE(m) (m[0])
#define LWMSG_GET_SEQNUM(m) (m[1])
#define LWMSG_GET_LEN(m) ( (ntohl(*((uint32_t*)(m)))&0xffff) )
#define LWMSG_GET_SESSIONID(m) ( ntohl(*  ( ( (uint32_t*)(m))[1])   ) )
#define LWMSG_GET_DATA(m) (m+8)


#define LWMSGELEM_GET_TYPE(m) (m[0])
#define LWMSGELEM_GET_LEN(m) (  (ntohl(*((uint32_t*)(m)))>>8)&0xffff )
#define LWMSGELEM_GET_DATA(m) (m+3)




#define lw_foreach_msgelem(d,msg,len) for(d=msg; d<msg+len; d=d+3+LWMSGELEM_GET_LEN(d))

//#define LWAPP_PACKET_PREAMBLE (CW_VERSION<<4)
//#define LWAPP_DTLS_PACKET_PREAMBLE (CW_VERSION<<4|1)



#define LWMSG_DISCOVERY_REQUEST		1
#define LWMSG_DISCOVERY_RESPONSE	2

/*                  Join Request                         3
                  Join Response                        4
                  Join ACK                             5
                  Join Confirm                         6
                  Unused                             7-9
                  Configure Request                   10
                  Configure Response                  11
                  Configuration Update Request        12
                  Configuration Update Response       13
                  WTP Event Request                   14
                  WTP Event Response                  15
                  Change State Event Request          16
                  Change State Event Response         17
                  Unused                           18-21
                  Echo Request                        22
                  Echo Response                       23
                  Image Data Request                  24
                  Image Data Response                 25
                  Reset Request                       26
                  Reset Response                      27
                  Unused                           28-29
                  Key Update Request                  30
                  Key Update Response                 31
                  Primary Discovery Request           32
*/


/* LWAPP message elements */

#define LWMSGELEM_WTP_DESCRIPTOR 		3

#define LWMSGELEM_SUPPORTED_RATES		16

#define LWMSGELEM_WTP_BOARD_DATA		50


/* function proto types */

uint16_t lw_checksum(uint8_t *d,int len);
int lw_readelem_wtp_board_data(struct wtpinfo *wtpinfo, int type, uint8_t *msgelem, int len);




#endif
