
#include <string.h>


#include "capwap/conn.h"
#include "capwap/capwap_80211.h"
#include "capwap/log.h"

#include "wtp_conf.h"
#include "wtp_interface.h"

/*
struct wtpinfo * get_wtpinfo()
{
	struct wtpinfo * wtpinfo;


	wtpinfo=malloc(sizeof(struct wtpinfo));
	memset(wtpinfo,0,sizeof(struct wtpinfo));

	wtpinfo->name = conf_wtpname;

	wtpinfo->location = (uint8_t*)"default location";

	wtpinfo->max_radios=wtpdrv_get_num_radios();
	int i;
	for (i=0; i<wtpdrv_get_num_radios(); i++){
		wtpdrv_get_radioinfo(i,&(wtpinfo->radioinfo[i]));

	}

	wtpinfo->radios_in_use=2;

	wtpinfo->encryption_cap=1;

	wtpinfo->serial_no=conf_serial_no;
	wtpinfo->vendor_id=conf_vendor_id;

	wtpinfo->model_no=conf_model_no;

	wtpinfo->bootloader_version="";
	wtpinfo->bootloader_vendor_id=CW_VENDOR_ID_CISCO;

	wtpinfo->hardware_version="\0\0";
	wtpinfo->hardware_vendor_id=CW_VENDOR_ID_CISCO;
	
	wtpinfo->software_version=conf_software_version;
	wtpinfo->hardware_version=conf_hardware_version;
	wtpinfo->bootloader_version=conf_bootloader_version;


	wtpinfo->board_id = conf_board_id;
	wtpinfo->board_revision = conf_board_revision;

	wtpinfo->software_vendor_id=CW_VENDOR_ID_CISCO;

	wtpinfo->macaddress=conf_macaddress;
	wtpinfo->macaddress_len=conf_macaddress_len;

	


	wtpinfo->max_msg_len=14000;

	wtpinfo->mac_type=1;


//	wtpinfo->session_id = malloc(8);
//	wtpinfo->session_id_len = cw_rand(wtpinfo->session_id,8);

	uint8_t sessid[4];
	memset(sessid,0,4);
	int sidl = cw_rand(sessid+2,2);
	
	wtpinfo->session_id = bstr_create(sessid,4);

	wtpinfo->frame_tunnel_mode=4;
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


static int sockfd = -1;
int get_sock()
{
	return sockfd;
	
}


int set_sock(int sock)
{
	sockfd = sock;	
}




static struct conn * conn=0;
struct conn * get_conn()
{
	if (!conn){
		conn = conn_create_noq(-1,0);
		if (conf_mtu){
			conn->mtu=conf_mtu;
		}
		conn->mtu_discovery=conf_mtu_discovery;	
	}
	printf("Get conn returns %p\n",conn);
	return conn;
}

*/



