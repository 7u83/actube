
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>


#include <uci.h>

#include "cw/capwap.h"
#include "cw/log.h"
#include "wtp_conf.h"

#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/bstr.h"


static struct uci_section  * get_anon_section(struct uci_package * pkg, const char *type)
{

	struct uci_element * e;
	struct uci_list * l = &pkg->sections;

	uci_foreach_element(l,e){
		struct uci_section *s = (struct uci_section*)e;
		if (!s->anonymous)
			continue;

		if (strcmp(s->type,type)==0){
			return s;
		}
	}
	return NULL;

}


static void set_dbg_opt(struct uci_context *ctx,struct uci_section * section,int opt,const char * optstr)
{

	const char *str = uci_lookup_option_string(ctx,section,optstr);
	if (!str){
		return;
	}

	if ((strcmp(str,"1")==0) || (strcmp(str,"true")==0))
		//conf_dbg_level |= opt;
		cw_dbg_set_level(opt,1);
	
	if ((strcmp(str,"0")==0) || (strcmp(str,"false")==0))
		cw_dbg_set_level(opt,0);


}


static void read_dbg_options(struct uci_context *ctx, struct uci_section *section)
{

        int i;
        for (i=0; cw_dbg_strings[i].id!=CW_STR_STOP; i++) {

		set_dbg_opt(ctx,section,cw_dbg_strings[i].id,cw_dbg_strings[i].str);

        }
}

static void read_timers(struct uci_context *ctx,struct uci_section *section)
{
        int i;
        for (i=0; conf_timer_cfgstrs[i].name; i++) {
		
		const char *str = uci_lookup_option_string(ctx,section,conf_timer_cfgstrs[i].name);
		if ( str ) {
			*(conf_timer_cfgstrs[i].value)=atol(str);

		}
	
        }

}

int read_config(const char * filename){


	struct uci_context * ctx;

	ctx = uci_alloc_context();
	if (!ctx){
		cw_log(LOG_ERR,"Fatal: Can't create uci ctx, can't read config file");
		return 0;
	}

	struct uci_package * pkg;
	
	if (filename == NULL){
		filename = "wtp_uci.conf";
	}
	cw_dbg(DBG_INFO,"Reading config file %s",filename);


	int rc = uci_load(ctx, filename, &pkg );

	if (rc == UCI_ERR_NOTFOUND){
		cw_log(LOG_INFO,"Config file '%s' not found, running without config",filename);
		return 1;
	}
	
	if (rc) {
		char * errstr; 
		uci_get_errorstr(ctx, &errstr, "");
		cw_log(LOG_ERR,"Fatal: Can't read config file: %s",errstr);
		return 0;
		
	}


	struct uci_section * section;

	section  = get_anon_section(pkg,"dbg");
	read_dbg_options(ctx,section);


	


	section = get_anon_section(pkg,"wtp");
	if (!section) {
		cw_dbg(DBG_INFO,"No 'wtp' section found, running without config");
		return 1;
	}

	read_timers(ctx,section);
	
	const char  *str;
	str = uci_lookup_option_string(ctx,section,"name");
	if (str)
		conf_wtpname = strdup(str);

	str = uci_lookup_option_string(ctx,section,"mtu");
	if (str)
		conf_mtu = atoi(str);

	str = uci_lookup_option_string(ctx,section,"mtu_discovery");
	if (str)
		conf_mtu_discovery = atoi(str);

	str = uci_lookup_option_string(ctx,section,"interface");
	if (str) 
		conf_primary_if=strdup(str);

	str = uci_lookup_option_string(ctx,section,"ip");
	if (str) 
		conf_ip=strdup(str);

	str = uci_lookup_option_string(ctx,section,"ssl_key");
	if (str) 
		conf_sslkeyfilename=strdup(str);

	str = uci_lookup_option_string(ctx,section,"ssl_cert");
	if (str) 
		conf_sslcertfilename=strdup(str);
	
	str = uci_lookup_option_string(ctx,section,"dtls_psk");
	if (str)
		conf_dtls_psk=strdup(str);

	str = uci_lookup_option_string(ctx,section,"ssl_cipher");
	if (str) 
		conf_dtls_cipher=strdup(str);


	str = uci_lookup_option_string(ctx,section,"vendor_id");
	if (str) 
		conf_vendor_id=atoi(str);

	str = uci_lookup_option_string(ctx,section,"echo_interval");
	if (str) 
		conf_echo_interval=atoi(str);


	str = uci_lookup_option_string(ctx,section,"software_version");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_software_version,s);
	}

	str = uci_lookup_option_string(ctx,section,"hardware_version");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_hardware_version,s);
	}

	str = uci_lookup_option_string(ctx,section,"bootloader_version");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_bootloader_version,s);
	}


	str = uci_lookup_option_string(ctx,section,"board_id");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_board_id,s);
	}

	str = uci_lookup_option_string(ctx,section,"board_revision");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_board_revision,s);
	}


	str = uci_lookup_option_string(ctx,section,"serial_no");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_serial_no,s);
	}


	str = uci_lookup_option_string(ctx,section,"model_no");
	if (str){
		uint8_t * s = bstr16_create_from_cfgstr(str);
		bstr16_replace(&conf_model_no,s);
	}

	


	
	return 1;

}



