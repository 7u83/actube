
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>


#include <uci.h>

#include "capwap/capwap.h"
#include "capwap/cw_log.h"
#include "wtp_conf.h"

#include "capwap/cw_log.h"

#include "capwap/bstr.h"


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
	if (!str)
		return;
	if ((strcmp(str,"1")==0) || (strcmp(str,"true")==0))
		conf_dbg_level |= opt;
	
	if ((strcmp(str,"0")==0) || (strcmp(str,"false")==0))
		conf_dbg_level &= ((-1)^opt);


}


static void read_dbg_options(struct uci_context *ctx, struct uci_section *section)
{

        int i;
        for (i=0; cw_dbg_cfgstrs[i].name; i++) {

		set_dbg_opt(ctx,section,cw_dbg_cfgstrs[i].level,cw_dbg_cfgstrs[i].name);

               // if (!strcmp(str,cw_dbg_cfgstrs[i].name))
                 //       return cw_dbg_cfgstrs[i].level;
        }
       // return 0;


/*	set_dbg_opt(ctx,section,DBG_DTLS,"dtls");
	set_dbg_opt(ctx,section,DBG_DTLS_DETAIL,"dtls_detail");
	set_dbg_opt(ctx,section,DBG_DTLS_BIO,"dtls_bio");
	set_dbg_opt(ctx,section,DBG_DTLS_BIO_DMP,"dtls_bio_dmp");
*/
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
	cw_dbg(DBG_ALL,"Reading config file %s",filename);


	int rc = uci_load(ctx, filename, &pkg );

	if (rc == UCI_ERR_NOTFOUND){
		cw_dbg(DBG_CW_INFO,"Config file '%s' not found, running without config",filename);
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
		cw_dbg(DBG_CW_INFO,"No 'wtp' section found, running without config");
		return 1;
	}

	
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

	str = uci_lookup_option_string(ctx,section,"ssl_key");
	if (str) 
		conf_sslkeyfilename=strdup(str);

	str = uci_lookup_option_string(ctx,section,"ssl_cert");
	if (str) 
		conf_sslcertfilename=strdup(str);

	str = uci_lookup_option_string(ctx,section,"vendor_id");
	if (str) 
		conf_vendor_id=atoi(str);

	str = uci_lookup_option_string(ctx,section,"software_version");
	if (str){
		uint8_t * s = bstr_create_from_cfgstr(str);
		bstr_replace(&conf_software_version,s);
	}

	str = uci_lookup_option_string(ctx,section,"model_no");
	if (str){
		uint8_t * s = bstr_create_from_cfgstr(str);
		bstr_replace(&conf_model_no,s);
	}


	
	return 1;

}



