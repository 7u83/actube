
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
//#include <confuse.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>


#include "cw/capwap.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/cw_util.h"

#include "wtp_conf.h"


#include "cw/sock.h"
#include "cw/log.h"

#include "cw/bstr.h"
#include "cw/vendors.h"

char * conf_primary_if=0;
char * conf_ip=0;
char * conf_wtpname=0;
char * conf_dtls_psk=0;
char * conf_sslkeyfilename=0;
char * conf_sslcertfilename=0;
char * conf_sslkeypass=0;


char * conf_preferred_ac=0;
char * conf_dtls_cipher=0;


struct sockaddr_storage * conf_preferred_ac_sa=0;


char ** conf_ac_list;
int conf_ac_list_len;
char * conf_control_port=0;

uint8_t conf_macaddress[12];
uint8_t conf_macaddress_len=0;

long conf_max_discovery_interval=CONF_DEFAULT_MAX_DISCOVERY_INTERVAL;
long conf_discovery_interval=CONF_DEFAULT_DISCOVERY_INTERVAL;
long conf_silent_interval=CONF_DEFAULT_SILENT_INTERVAL;
long conf_max_discoveries=CONF_DEFAULT_MAX_DISCOVERIES;
long conf_echo_interval=CONF_DEFAULT_ECHO_INTERVAL;
long conf_max_retransmit=CONF_DEFAULT_MAX_RETRANSMIT;
long conf_retransmit_interval=CONF_DEFAULT_RETRANSMIT_INTERVAL;

//long conf_dbg_level=CONF_DEFAULT_DEBUG_LEVEL;



int conf_mtu_discovery=CONF_DEFAULT_MTU_DISCOVERY;
int conf_mtu=0;


uint32_t conf_vendor_id;
uint32_t * conf_bootloader_vendor_id;
uint8_t * conf_bootloader_version;
uint32_t * conf_software_vendor_id;
uint8_t * conf_software_version;
uint32_t * conf_hardware_vendor_id;
uint8_t * conf_hardware_version;

uint8_t * conf_model_no;
uint8_t * conf_serial_no;



bstr_t conf_board_id;
bstr_t conf_board_revision;




LONGSTRS conf_timer_cfgstrs[] = {
	{"max_discovery_interval",&conf_max_discovery_interval},
	{"discovery_interval",&conf_discovery_interval},
	{"max_discoveries",&conf_max_discoveries},
	{"silent_interval",&conf_silent_interval},
	{0,0}
};


int wtpconf_primary_if()
{

	if (!conf_primary_if ) {
	        conf_primary_if  = sock_get_primary_if(AF_INET6);
	        if (!conf_primary_if)
	                conf_primary_if = sock_get_primary_if(AF_INET);
	}


        if (!conf_primary_if){
                cw_log(LOG_ERR,"Fatal: Unable to detect primary interface");
                return 0;
        }               

        if (!sock_getifhwaddr(conf_primary_if,conf_macaddress,&conf_macaddress_len)){


                cw_log(LOG_ERR,"Fatal: Unable to detect link layer address for %s:",conf_primary_if, 
				strerror(errno));
                return 0;
        };
	
	cw_dbg(DBG_INFO, "Primary interface: %s, mac address: %s.",
			conf_primary_if,
			sock_hwaddr2str(conf_macaddress,conf_macaddress_len)
			);
	return 1;
}

int wtpconf_name()
{
	if (conf_wtpname)
		return 1;
	
	char name[64];
	sprintf(name,"WTP%s",sock_hwaddr2idstr(conf_macaddress,conf_macaddress_len));

	conf_wtpname = (char*)cw_setstr((uint8_t**)&conf_wtpname,(uint8_t*)name,strlen(name)); 
	if (!conf_wtpname)
		return 0;

	cw_dbg(DBG_INFO,"Using self assigned wtp name: %s",conf_wtpname);

	return 1;
}


#include <netinet/in.h>




char * default_ac_list[] = {
//	"192.168.0.255",
	"255.255.255.255"
//	"224.0.1.140",
	//"192.168.0.77"
	//"192.168.56.99"
};

int wtpconf_ac_list()
{
	if (conf_ac_list)
		return 1;

	int i;
	int len=0;
	int bcrc;
	struct sockaddr_storage bcaddr;

	bcrc = sock_getifaddr(conf_primary_if,AF_INET,IFF_BROADCAST,(struct sockaddr*)&bcaddr);
	if (bcrc)
		len++;

	int deflen = sizeof(default_ac_list)/sizeof(char*);

	len += deflen;
	conf_ac_list = malloc(len*sizeof(char*));
	if (!conf_ac_list)
		return 0;

	for (i=0; i<deflen; i++){
		conf_ac_list[i]=strdup(default_ac_list[i]);
		if (!conf_ac_list[i])
			return 0;
	}

	if (bcrc){
//		char bcstr[100];
		char *bcstr;
//		sock_addrtostr((struct sockaddr*)&bcaddr,bcstr,100);
		bcstr = sock_addr2str((struct sockaddr*)&bcaddr); 
//		char * c = strchr(bcstr,':');
//		*c=0;
		conf_ac_list[i]=strdup(bcstr);
	}

	conf_ac_list_len=len;

#ifdef WITH_CW_LOG_DEBUG
	for (i=0; i<conf_ac_list_len; i++){
		cw_dbg(DBG_INFO,"Using AC: %s",conf_ac_list[i]);
	}
#endif

	return 1;
}






int wtpconf_preinit()
{
	conf_control_port=strdup(CAPWAP_CONTROL_PORT_STR);
	conf_dtls_cipher=strdup(CONF_DEFAULT_DTLS_CIPHER);


	conf_vendor_id = CONF_DEFAULT_VENDOR_ID;
	conf_software_version = bstr_create_from_cfgstr(CONF_DEFAULT_SOFTWARE_VERSION);
	conf_hardware_version = bstr_create_from_cfgstr(CONF_DEFAULT_HARDWARE_VERSION);
	conf_bootloader_version = bstr_create_from_cfgstr(CONF_DEFAULT_BOOTLOADER_VERSION);
	conf_serial_no = bstr_create_from_cfgstr(CONF_DEFAULT_SERIAL_NO);
	conf_model_no = bstr_create_from_cfgstr(CONF_DEFAULT_MODEL_NO);

	conf_board_id = bstr_create_from_cfgstr(CONF_DEFAULT_BOARD_ID);
	conf_board_revision = bstr_create_from_cfgstr(CONF_DEFAULT_BOARD_REVISION);
	return 0;
}

int wtpconf_init()
{


	if (!wtpconf_primary_if()){
		cw_log(LOG_ERR,"Fatal: Error initialing primary interface.");
		goto errX;
	}

	if (!wtpconf_ac_list()){
		cw_log(LOG_ERR,"Fatal: Error initialiing ac list.");
		goto errX;
	}

	if (!wtpconf_name()){
		cw_log(LOG_ERR,"Fatal: Cant't set wtp name.");
		goto errX;
	}

	return 1;

errX:
	return 0;



/*


	if (conf_preferred_ac){
		conf_preferred_ac_sa=malloc(sizeof(struct sockaddr_storage));
		if (sock_strtoaddr(conf_preferred_ac,(struct sockaddr*)conf_preferred_ac_sa)!=1)
		{
			cw_log(LOG_ERR,"Preferred AC, invalid address: %s",conf_preferred_ac);
			free(conf_preferred_ac_sa);
			conf_preferred_ac_sa=0;
		}
		else{
			if (sock_getport((struct sockaddr*)conf_preferred_ac_sa)==0){
				sock_setport((struct sockaddr*)conf_preferred_ac_sa,conf_control_port);
			}
		}
		if (conf_preferred_ac_sa!=0){
			char str[100];
			sock_addrtostr((struct sockaddr*)conf_preferred_ac_sa,str,100);
			cw_log(LOG_INFO,"Preferred AC: %s\n",str);
		}
	}
*/
	
}

/*
int read_config(const char * filename){
	int i,n;


	conf_control_port=strdup(CAPWAP_CONTROL_PORT_STR);
	conf_dtls_cipher=strdup(CONF_DEFAULT_DTLS_CIPHER);

	cfg_opt_t opts[] = {
		CFG_SIMPLE_STR("wtp_name",&conf_wtpname),
		CFG_SIMPLE_STR("dtls_psk",&conf_dtls_psk),
		CFG_SIMPLE_STR("preferred_ac",&conf_preferred_ac),
		CFG_SIMPLE_STR("primary_if",&conf_primary_if),
		CFG_SIMPLE_STR("dtls_cipher",&conf_dtls_cipher),
		CFG_STR_LIST("ac_list", "{}", CFGF_NONE),
		CFG_SIMPLE_STR("control_port",&conf_control_port),
		CFG_SIMPLE_INT("discovery_interval",&conf_discovery_interval),
		CFG_SIMPLE_INT("max_discovery_interval",&conf_max_discovery_interval), 
		CFG_SIMPLE_INT("max_discoveries",&conf_max_discoveries),
		CFG_SIMPLE_INT("silent_interval",&conf_silent_interval),
		CFG_SIMPLE_INT("debug_level",&conf_debug_level),
		CFG_SIMPLE_INT("max_retransmit",&conf_max_retransmit),
		CFG_SIMPLE_INT("retransmit_interval",&conf_retransmit_interval),
		CFG_SIMPLE_INT("echo_interval",&conf_echo_interval),


		CFG_END()
	};

        cfg_t *cfg;
        cfg = cfg_init(opts, 0);

        cfg_parse(cfg, filename);


	n = cfg_size(cfg, "ac_list");
	if (n>0)
	{
		conf_ac_list=malloc(sizeof(char*)*n);
		if (!conf_ac_list) 
			goto errX;

		conf_ac_list_len=0;
		for (i=0; i<n; i++){
			char * str = cfg_getnstr(cfg,"ac_list",i);
			conf_ac_list[conf_ac_list_len]=malloc(strlen(str)+1);
			if(conf_ac_list) {
				strcpy(conf_ac_list[conf_ac_list_len],str);
				conf_ac_list_len++;
			}
		}
	}



	


	if (!wtpconf_primary_if())
		goto errX;

	if (!wtpconf_ac_list()){
		printf("ac list error\n");
		goto errX;
	}


//	wtpconf_init();
	return 1;

errX:
	cfg_free(cfg);
	return 0;

}


*/
