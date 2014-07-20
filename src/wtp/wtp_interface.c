
#include <string.h>


#include "capwap/wtpinfo.h"
#include "capwap/acinfo.h"
#include "capwap/conn.h"

#include "wtp_conf.h"
#include "wtp_interface.h"


struct wtpinfo * get_wtpinfo()
{
	struct wtpinfo * wtpinfo;
	wtpinfo=malloc(sizeof(struct wtpinfo));
	memset(wtpinfo,0,sizeof(struct wtpinfo));

	wtpinfo->name = (uint8_t*)"wtp";
	wtpinfo->location = (uint8_t*)"Unknown";

	wtpinfo->max_radios=wtpdrv_get_num_radios();
/*	int i;
	for (i=0; i<wtpdrv_get_num_radios(); i++){
		wtpdrv_get_radioinfo(i+1,&wtpinfo.radioinfo[i+1]);

	}
*/
	wtpinfo->serial_no="123456789";
	wtpinfo->vendor_id=909090;

	wtpinfo->model_no="WNDR 3700";

	wtpinfo->bootloader_version="1.0";
	wtpinfo->bootloader_vendor_id=12345;

	wtpinfo->hardware_version="2.5";
	wtpinfo->hardware_vendor_id=909090;
	
	wtpinfo->software_version="2.0";
	wtpinfo->software_vendor_id=109090;

	wtpinfo->macaddress=conf_macaddress;
	wtpinfo->macaddress_len=conf_macaddress_len;

	wtpinfo->mac_type=1;


	wtpinfo->session_id = malloc(8);
	wtpinfo->session_id_len = cw_rand(wtpinfo->session_id,8);


	wtpinfo->frame_tunnel_mode=0;
	return wtpinfo;
}

void destroy_wtpinfo(struct wtpinfo* wtpinfo)
{

}


ACIPLIST * order_aciplist(ACIPLIST *aciplistin)
{
//	ACIPLIST * aciplist = aciplist_create();

//	aciplist_foreach(aciplistin);

	return aciplistin;
}


static struct conn * conn=0;
struct conn * get_conn()
{
	if (!conn){
		conn = conn_create_noq(-1,0);
	}
	return conn;
}
