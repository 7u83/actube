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

int radioinfo_print(char * str, struct radioinfo * radioinfo)
{
	char * s = str;

	s += sprintf(s,"\t  RID %d\n",radioinfo->rid);
	s += sprintf(s,"\t  Modes: ");
	if(radioinfo->type & CWRADIO_TYPE_B){
		s+=sprintf(s,"B");
	}
	if(radioinfo->type & CWRADIO_TYPE_G){
		s+=sprintf(s,"G");
	}

	if(radioinfo->type & CWRADIO_TYPE_A){
		s+=printf(s,"A");
	}

	if(radioinfo->type & CWRADIO_TYPE_N){
		s+=sprintf(s,"N");
	}

	s+=sprintf(s,"\n");

	return s-str;

}


static int version_print(char *s, const uint8_t *version, int len, uint32_t vendor)
{
	
	if (!version)
		return sprintf(s,"Not set\n");

	int rs=0;	
	int i;
	for (i=0; i<len && i<20; i++){
		rs+=sprintf(s+rs,"%02X",version[i]);
	}

	int dot=0;

	rs+=sprintf(s+rs," (");
	for (i=0; i<len && i<20; i++){
		if (dot) 
			rs+=sprintf(s+rs,".");
		dot=1;
		rs+=sprintf(s+rs,"%d",version[i]);
	}
	rs+=sprintf(s+rs,")");


	rs+=sprintf(s+rs,", Vendor Id: %d",vendor);
	rs+=sprintf(s+rs,"\n");
	return rs;	
	
}

int wtpinfo_print(char *str, struct wtpinfo * wtpinfo)
{
	char hstr[64];

	char *s = str;




	s+=sprintf (s,"\tWTP Name: %s\n", (!wtpinfo->name ? (uint8_t*)"Not set" : wtpinfo->name) );
	s+=sprintf (s,"\tLocation: %s\n", (!wtpinfo->location ? (uint8_t*)"Not set" : wtpinfo->location) );

	s+=sprintf (s,"\tMAC Adress: ");
	if (wtpinfo->macaddress){
		sock_hwaddrtostr(wtpinfo->macaddress,wtpinfo->macaddress_len,hstr,":");
		s+=sprintf(s,"%s\n",hstr);

	}
	else
		s+=sprintf(s,"Not set\n");

	char disctypestr[32];
	switch(wtpinfo->discovery_type){
		case CW_DISCOVERY_TYPE_STATIC:
			sprintf(disctypestr,"Static");
			break;
			
		case CW_DISCOVERY_TYPE_DHCP:
			sprintf(disctypestr,"DHCP");
			break;
			
		case CW_DISCOVERY_TYPE_DNS:
			sprintf(disctypestr,"DNS");
			break;

		case CW_DISCOVERY_TYPE_AC_REFERRAL:
			sprintf(disctypestr,"AC Referral");
			break;

		default:
			sprintf(disctypestr,"Unknown");
			break;

	}
	s+=sprintf (s,"\tDiscovery Type: %s\n",disctypestr);



	sock_addrtostr((struct sockaddr*)&wtpinfo->local_ip,hstr,64);
	s+=sprintf (s,"\tLocal IP: %s\n",hstr);



	s+=sprintf (s,"\tVendor ID: %d\n", wtpinfo->vendor_id );
	s+=sprintf (s,"\tModel No.: %s\n", (!wtpinfo->model_no ? (uint8_t*)"Not set" : wtpinfo->model_no) );
	s+=sprintf (s,"\tSerial No.: %s\n", (!wtpinfo->serial_no ? (uint8_t*)"Not set" : wtpinfo->serial_no) );

	s+=sprintf (s,"\tSoftware Version: ");
	s+=version_print(s,wtpinfo->software_version,wtpinfo->software_version_len,wtpinfo->software_vendor_id);
	s+=sprintf (s,"\tHardware Version: ");
	s+=version_print(s,wtpinfo->hardware_version,wtpinfo->hardware_version_len,wtpinfo->hardware_vendor_id);
	s+=sprintf (s,"\tBootloader Version: ");
	s+=version_print(s,wtpinfo->bootloader_version,wtpinfo->bootloader_version_len,wtpinfo->bootloader_vendor_id);
		



//, (!wtpinfo->software_version ? (uint8_t*)"Not set" : wtpinfo->software_version) );



//	s+=sprintf (s,"\tHardware Version: %s\n", (!wtpinfo->hardware_version ? (uint8_t*)"Not set" : wtpinfo->hardware_version) );


	s+=sprintf (s,"\tMax Radios: %d\n",wtpinfo->max_radios);
	s+=sprintf (s,"\tRadios in use: %d\n",wtpinfo->radios_in_use);


	s+=sprintf (s,"\tSession ID: ");
	if (wtpinfo->session_id) {
		int i;
		for (i=0; i<wtpinfo->session_id_len; i++)
			s+=sprintf(s,"%02X",wtpinfo->session_id[i]);
	}
	else 
		s+=sprintf(s,"Not set");
		s+=sprintf(s,"\n");

	s+=sprintf (s,"\tMAC Type: ");
	switch (wtpinfo->mac_type){
		case WTP_MAC_TYPE_LOCAL:
			s+=sprintf(s,"local");
			break;
		case WTP_MAC_TYPE_SPLIT:
			s+=sprintf(s,"split");
			break;
		case WTP_MAC_TYPE_BOTH:
			s+=sprintf(s,"local, split");
			break;
	}
	s+=sprintf(s,"\n");

	s+=sprintf (s,"\tFrame Tunnel Mode: ");
	s+=sprintf(s,"(%08X)",wtpinfo->frame_tunnel_mode);
	char * c="";
	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_N){
		s+=printf (s,"%snative",c);c=", ";
	}

	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_E){
		s+=sprintf (s,"%s802.3",c);c=", ";
	}

	if (wtpinfo->frame_tunnel_mode & WTP_FRAME_TUNNEL_MODE_L){
		s+=sprintf (s,"%sLocal bridging",c);c=", ";
	}
	if (wtpinfo->frame_tunnel_mode == 0)
		s+=sprintf(s," None");

	s+=sprintf(s,"\n");

	s+=sprintf(s,"\tRadios: %d\n",wtpinfo->max_radios);	
	int i;

	char ristr[2048];
	char *r = ristr;
	for (i=0; i<wtpinfo->max_radios; i++){
		r+=radioinfo_print(r,&wtpinfo->radioinfo[i+1]);
	}

	s+=sprintf(s,"%s",ristr);

	return s-str;


}
