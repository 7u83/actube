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

#include <stdio.h>
#include <stdlib.h>

#include "capwap.h"
#include "sock.h"

void radioinfo_print(struct radioinfo * radioinfo)
{
	printf("  RID %d\n",radioinfo->rid);
	printf("  Modes: ");
	if(radioinfo->type & CWRADIO_TYPE_B){
		printf("B");
	}
	if(radioinfo->type & CWRADIO_TYPE_G){
		printf("G");
	}

	if(radioinfo->type & CWRADIO_TYPE_A){
		printf("A");
	}

	if(radioinfo->type & CWRADIO_TYPE_N){
		printf("N");
	}

	printf("\n");


}

void wtpinfo_print(struct wtpinfo * wtpinfo)
{
	char str[64];
	printf ("=== WTP Info: %p ===\n",wtpinfo);
	printf (" Name:\t\t\t%s\n", (!wtpinfo->name ? (uint8_t*)"Not set" : wtpinfo->name) );
	printf (" Location:\t\t%s\n", (!wtpinfo->location ? (uint8_t*)"Not set" : wtpinfo->location) );

	printf (" MAC Adress:\t\t");
	if (wtpinfo->macaddress){
		sock_hwaddrtostr(wtpinfo->macaddress,wtpinfo->macaddress_len,str);
		printf("%s\n",str);

	}
	else
		printf("Not set\n");

	sock_addrtostr((struct sockaddr*)&wtpinfo->local_ip,str,64);
	printf (" Local IP:\t\t%s\n",str);


	printf (" Vendor ID:\t\t%d\n", wtpinfo->vendor_id );
	printf (" Model No.:\t\t%s\n", (!wtpinfo->model_no ? (uint8_t*)"Not set" : wtpinfo->model_no) );
	printf (" Serial No.:\t\t%s\n", (!wtpinfo->serial_no ? (uint8_t*)"Not set" : wtpinfo->serial_no) );

	printf (" Software Version:\t%s\n", (!wtpinfo->software_version ? (uint8_t*)"Not set" : wtpinfo->software_version) );


	printf (" Max Radios:\t\t%d\n",wtpinfo->max_radios);
	printf (" Radios in use:\t\t%d\n",wtpinfo->radios_in_use);


	printf (" Session ID:\t\t");
	if (wtpinfo->session_id) {
		int i;
		for (i=0; i<wtpinfo->session_id_len; i++)
			printf("%02X",wtpinfo->session_id[i]);
	}
	else 
		printf ("Not set");
		printf("\n");

	printf (" MAC Type:\t\t");
	switch (wtpinfo->mac_type){
		case WTP_MAC_TYPE_LOCAL:
			printf("local");
			break;
		case WTP_MAC_TYPE_SPLIT:
			printf("split");
			break;
		case WTP_MAC_TYPE_BOTH:
			printf("local, split");
			break;
	}
	printf("\n");

	printf (" Frame Tunnel Mode:\t");
	printf("(%08X)",wtpinfo->frame_tunnel_mode);
	char * c="";
	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_N){
		printf ("%snative",c);c=", ";
	}

	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_E){
		printf ("%s802.3",c);c=", ";
	}

	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_L){
		printf ("%sLocal bridging",c);c=", ";
	}
	if (wtpinfo->frame_tunnel_mode == 0)
		printf(" None");

	printf("\n");

	printf(" Radios: %d\n",wtpinfo->max_radios);	
	int i;
	for (i=0; i<wtpinfo->max_radios; i++){
		radioinfo_print(&wtpinfo->radioinfo[i+1]);
	}



}
