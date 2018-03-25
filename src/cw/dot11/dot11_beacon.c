#include "dot11.h"




int dot11_create_beacon(uint8_t *dst)
{
	uint8_t *d = dst;
	d+=dot11_put_frame_control(d,DOT11_FC_BEACON,0);
	d+=dot11_put_duration(d,0);
	d+=dot11_put_address(d,DOT11_BROADCAST_ADDRESS);
	uint8_t addr[6]={0x00,0x3a,0x99,0x02,0xfa,0xc0};
	d+=dot11_put_address(d,addr);
	d+=dot11_put_address(d,addr);
	d+=dot11_put_sequence_control(d,17);

	d+=dot11_put_timestamp(d);
	d+=dot11_put_beacon_interval(d,100);
	d+=dot11_put_capability(d,DOT11_CAP_ESS | DOT11_CAP_SHORT_PREAMBLE);
	d+=dot11_put_ssid(d,(uint8_t*)"Hello Word",10);


	float brates[] = {2,4,5.5,0.0};
	float rates[] = {1,4,54,0.0};

	d+=dot11_put_supported_rates(d,brates,rates);
	d+=dot11_put_dsss_param_set(d,1);




	return 0;
}

