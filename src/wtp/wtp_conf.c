
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
//#include <confuse.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>


#include "capwap/capwap.h"
#include "capwap/cw_log.h"
#include "capwap/cw_util.h"

#include "wtp_conf.h"


#include "capwap/sock.h"

char * conf_primary_if=0;
char * conf_wtpname=0;
char * conf_dtls_psk=0;
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

long conf_debug_level=CONF_DEFAULT_DEBUG_LEVEL;






int wtpconf_primary_if()
{

#ifdef WITH_IPV6
        conf_primary_if  = sock_get_primary_if(AF_INET6);
        if (!conf_primary_if)
                conf_primary_if = sock_get_primary_if(AF_INET);
#else   
        conf_primary_if = get_primary_if(AF_INET);
#endif

        if (!conf_primary_if){
                cw_log(LOG_ERR,"Fatal: Unable to detect primary interface");
                return 0;
        }               

        if (!sock_getifhwaddr(conf_primary_if,conf_macaddress,&conf_macaddress_len)){
                cw_log(LOG_ERR,"Fatal: Unable to detect link layer address for %s.",conf_primary_if);
                return 0;
        };
	
	cw_log_debug0("Primary interfac: %s, mac address: %s.",
			conf_primary_if,
			hwaddr2str(conf_macaddress,conf_macaddress_len)
			);


	return 1;

}

int wtpconf_name()
{
	if (conf_wtpname)
		return 1;
	
	char name[64];
	sprintf(name,"WTP%s",hwaddr2idstr(conf_macaddress,conf_macaddress_len));

	conf_wtpname = (char*)cw_setstr((uint8_t**)&conf_wtpname,(uint8_t*)name,strlen(name)); 
	if (!conf_wtpname)
		return 0;

	cw_log_debug0("Using self assigned wtp name: %s",conf_wtpname);

	return 1;
}


#include <netinet/in.h>




char * default_ac_list[] = {
	"255.255.255.255",
	"224.0.1.140",
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
		char bcstr[100];
		sock_addrtostr((struct sockaddr*)&bcaddr,bcstr,100);
		char * c = strchr(bcstr,':');
		*c=0;
		conf_ac_list[i]=strdup(bcstr);
	}

	conf_ac_list_len=len;
#ifdef WITH_CW_LOG_DEBUG
	for (i=0; i<conf_ac_list_len; i++){
		cw_log_debug0("Using AC: %s\n",conf_ac_list[i]);
	}
#endif

	return 1;
}






int wtpconf_preinit()
{
	conf_control_port=strdup(CAPWAP_CONTROL_PORT_STR);
	conf_dtls_cipher=strdup(CONF_DEFAULT_DTLS_CIPHER);

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
