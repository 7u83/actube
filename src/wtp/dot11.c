/*
    This file is part of actube-wtp.

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
#include <endian.h>

#include "dot11.h"

/*
uint8_t *  mk_frame( uint8_t *b)
{

}


int add_bytes(uint8_t * b, uint8_t *bytes, int len)
{
	memcpy(b,bytes,len);
	return len;
} 


uint16_t get_capab_info(struct apdata *ap)
{
	uint16_t capab=0;

	capab |= WLAN_CAPAB_ESS;
	
	return capab;	
}


int add_ssid_ie(uint8_t *b, const uint8_t *ssid, uint8_t ssid_len)
{
	*b = WLAN_EID_SSID;
	*(b+1) = ssid_len;
	memcpy(b+2,ssid,ssid_len);
	return ssid_len+2;
}


int add_ds_ie(uint8_t *b)
{
	*b = WLAN_EID_DS_PARAMS;
	*(b+1) = 1;
	*(b+2) = 6;
	return 3;

}


int dot11_get_beacon_head(struct apdata *ap, uint8_t *dst, int *len)
{
	uint8_t * buffer = dst; 
	uint8_t *b = buffer;

printf("t1\n");

	// set-up head
	struct dot11_mgmt_head head;
	memset (&head,0,sizeof(head));

printf("t2\n");

	head.frame_control = htole16 (DOT11_FTYPE_MGMT | DOT11_STYPE_BEACON); 
	head.duration = htole16(0);

	// destination address 
	memset(head.da, 0xff, sizeof(head.da));
printf("t3\n");

	// source address 
	memcpy (head.sa , ap->mac,6);
	memcpy (head.bssid , ap->mac,6);
	b += add_bytes(b,&head,sizeof(head));

printf("t4\n");

	struct dot11_mgmt_beacon beacon;
	memset (&beacon,0,sizeof(beacon));
	beacon.capab_info=htole16(get_capab_info(ap));
	beacon.beacon_int=htole16(0);
	b += add_bytes(b,&beacon,sizeof(beacon));

printf("t5 ss %s\n",ap->ssid);

	b += add_ssid_ie(b,ap->ssid,strlen(ap->ssid));
printf("t6\n");

	b+= add_ds_ie(b);
printf("return ay<aa\n");


int rates[] = {
	2000, 2500, 11000
};

	
	b+= add_supp_rates_ie(b,rates,3);

	*len = b-buffer;	

	return b-buffer;	
}




add_supp_rates_ie(uint8_t *buf,uint32_t * rates, int num_rates)
{
	uint8_t *b = buf;
	int len = num_rates;

	*b++ = WLAN_EID_SUPP_RATES;
	*b++ = len;
	
	int i;
	for (i=0; i<len; i++)
	{
		uint8_t r = rates[i]/500;
		r|=0x80;

		*b++ = r;
	}

	return b-buf;

}
*/

/*
dot11_get_beacon_data(struct apdata *ap,struct beacon_data *bd)
{
	bd->head = malloc(256);

	dot11_get_beacon_head(ap,bd->head,&bd->head_len);

uint8_t hf[] = {
0x80, 0x00, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x7B,
0xCB, 0xA4, 0xF2, 0x50, 0xF0, 0x7B, 0xCB, 0xA4, 0xF2, 0x50, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x21, 0x04, 
0x00, 0x09, 0x58, 0x54, 0x61, 0x74, 0x6F, 0x72, 0x74, 0x37, 0x37, 0x01, 
0x08, 0x82, 0x84, 0x8B, 0x96, 0x0C, 0x12, 0x18, 0x24

};

printf ("memcpy head %d\n",sizeof(hf));
	memcpy(bd->head,hf,sizeof(hf));
printf(" memcpyed\n");
	bd->head_len=sizeof(hf);
printf ("done\n");
	return 0;
	
}
*/

