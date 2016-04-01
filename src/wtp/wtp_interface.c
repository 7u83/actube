
#include <string.h>


#include "cw/conn.h"
//#include "cw/capwap_80211.h"
#include "cw/log.h"

#include "wtp_conf.h"
#include "wtp_interface.h"




bstr_t get_base_rmac()
{

//	static uint8_t rm[8] = { 0x00, 0x4a, 0x99, 0x02, 0xfa, 0xc0 };
//	static  uint8_t rm[8]={0x00,0x3a,0x99,0x02,0xfa,0xc0};
	static  uint8_t rm[8]={0x00,0x3a,0x99,0x04,0xfa,0xc0};

	return bstr_create(rm, 6);
}


uint8_t get_ecn_support()
{
	return 0;
}
