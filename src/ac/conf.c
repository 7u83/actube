/*
    This file is part of actube.

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

#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#include <confuse.h>

#include "cw/capwap.h"
#include "cw/sock.h"

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/cw_util.h"
#include "cw/dtls.h"

     #include <sys/param.h>
     #include <sys/time.h>
     #include <sys/socket.h>
     #include <net/if.h>

#include "conf.h"
#include "ac.h"

#include "cw/mavltypes.h"

uint8_t conf_macaddress[12];
uint8_t conf_macaddress_len = 0;


long conf_strict_capwap = 1;
long conf_strict_headers = 0;
/*int conf_capwap_mode = CW_MODE_CAPWAP;*/



const char *conf_acname = NULL;
int conf_acname_len = 0;

char *conf_acid = NULL;

char *conf_primary_if = NULL;

long conf_max_wtps = CONF_DEFAULT_MAXWTPS;
char *conf_logfilename = CONF_DEFAULT_LOGFILENAME;
struct sockaddr_storage *conf_salist = NULL;

char **conf_listen_addrs;
int conf_listen_addrs_len = 0;


char **conf_mcast_groups = 0;
int conf_mcast_groups_len = 0;

char **conf_bcast_addrs = 0;
int conf_bcast_addrs_len;


struct sockaddr_storage *conf_bsalist = NULL;

int conf_salist_len = 0;
int conf_bsalist_len = 0;

struct sockaddr *conf_ac_ips;
int conf_ac_ips_len;

char *conf_sslcertfilename = NULL;
char *conf_sslkeyfilename = NULL;
char *conf_sslkeypass = NULL;
char *conf_sslcipher = NULL;
char *conf_dtls_psk = NULL;

int conf_security = 0;
long conf_vendor_id = CONF_DEFAULT_VENDOR_ID;


bstr_t conf_hardware_version = NULL;
bstr_t conf_software_version = NULL;

bstr_t conf_cisco_hardware_version = NULL;
bstr_t conf_cisco_software_version = NULL;


int conf_use_loopback = 0;

long conf_debug_level = -1;

char *conf_db_file = 0;

int conf_ipv4 = 1;
#ifdef WITH_IPV6
int conf_ipv6 = 1;
#endif


#ifdef WITH_LWAPP
int conf_lwapp = 1;
char *conf_lw_control_port = 0;
#endif

char *conf_image_dir = 0;


char *conf_control_port = 0;


int conf_dtls_verify_peer = 1;

static int init_acname()
{
	if (conf_acname == NULL) {
		char *s = malloc(strlen(CONF_DEFAULT_ACNAME) + strlen(conf_acid) + 1);
		sprintf(s, "%s%s", CONF_DEFAULT_ACNAME, conf_acid);
		conf_acname = s;
	}
	conf_acname_len = strlen(conf_acname);
	return 1;
}

#include "../mod/modload.h"




static int init_acid()
{
	int i;
	char *s;
	
	if (conf_acid != NULL)
		return 1;


#ifdef WITH_IPV6
	conf_primary_if = sock_get_primary_if(AF_INET6);
	if (!conf_primary_if)
		conf_primary_if = sock_get_primary_if(AF_INET);
#else
	conf_primary_if = get_primary_if(AF_INET);
#endif

	if (!conf_primary_if) {
		cw_log(LOG_ERR,
		       "Fatal: Unable to detect primary interface, needed to set ac_id. Pleas use confiPleas u to set ac_id");
		return 0;
	}

	if (!sock_getifhwaddr(conf_primary_if, conf_macaddress, &conf_macaddress_len)) {
		cw_log(LOG_ERR, "Fatal: Unable to detect link layer address for %s\n",
		       conf_primary_if);
		return 0;
	};




	conf_acid = malloc(2 * conf_macaddress_len + 1);
	s = conf_acid;

	for (i = 0; i < conf_macaddress_len; i++) {
		s += sprintf(s, "%02X", conf_macaddress[i]);
	}
	return 1;

}


static int init_dtls()
{
	if (conf_dtls_psk != NULL) {
/*
		//              conf_security=CW_SECURITY_FLAGS_S;
*/
	}

	return 1;
}


static int init_vendor_id()
{
	return 1;
}

static int init_control_port()
{
	char str[30];
	sprintf(str, "%d", CONF_DEFAULT_CONTROL_PORT);
	conf_control_port =
	    (char *) cw_setstr((uint8_t **) & conf_control_port, (uint8_t *) str,
			       strlen(str));


#ifdef WITH_LWAPP
	sprintf(str, "%d", CONF_DEFAULT_LW_CONTROL_PORT);
	conf_lw_control_port = strdup(str);
#endif

	return 1;
}

#include <ifaddrs.h>

static int init_listen_addrs()
{
	struct ifaddrs *ifap, *ifa;
	int rc;
	int ctr;
	
	if (conf_listen_addrs != 0)
		return 1;



	rc = getifaddrs(&ifap);
	if (rc == -1)
		return 0;

	/* count the addresses */
	ctr = 0;
	for (ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET && conf_ipv4)
			ctr++;
		if (ifa->ifa_addr->sa_family == AF_INET6 && conf_ipv6)
			ctr++;
	}

	conf_listen_addrs = malloc(sizeof(char *) * ctr);
	if (!conf_listen_addrs) {
		rc = 0;
		goto errX;
	}
	memset(conf_listen_addrs, 0, sizeof(char *) * ctr);


	ctr = 0;
	/* get the addresses */
	for (ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		char str[100];

		/** ignore loopback interfaces if configured*/
		if (!conf_use_loopback) {
			if ((ifa->ifa_flags & IFF_LOOPBACK)) {
				continue;
			}
		}
		if (!ifa->ifa_addr)
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET && conf_ipv4) {
			sock_addrtostr(ifa->ifa_addr, str, 100,0);

			conf_listen_addrs[ctr] =
			    (char *) cw_setstr((uint8_t **) & conf_listen_addrs[ctr],
					       (uint8_t *) str, strlen(str));
			if (conf_listen_addrs[ctr])
				ctr++;

		}
		if (ifa->ifa_addr->sa_family == AF_INET6 && conf_ipv6) {
			sock_addrtostr(ifa->ifa_addr, str, 100,0);

			if (strncmp(str, "fe80:", 5) == 0) {
				strcat(str, "%");
				strcat(str, ifa->ifa_name);
			}
			conf_listen_addrs[ctr] =
			    (char *) cw_setstr((uint8_t **) & conf_listen_addrs[ctr],
					       (uint8_t *) str, strlen(str));
			if (conf_listen_addrs[ctr])
				ctr++;
		}
	}
	conf_listen_addrs_len = ctr;
	rc = 1;
      errX:
	freeifaddrs(ifap);
	return rc;
}


static char *conf_default_mcast_groups_ipv4[] = {
	"224.0.1.140",
};



#ifdef WITH_IPV6
static char *conf_default_mcast_groups_ipv6[] = {
	"ff01:0:0:0:0:0:0:18c",
/*	"ff01:0:0:0:0:0:0:18c",
	"ff02:0:0:0:0:0:0:18c%em0",
	"ff03:0:0:0:0:0:0:18c",
	"ff04:0:0:0:0:0:0:18c",
	"ff05:0:0:0:0:0:0:18c",
	"ff06:0:0:0:0:0:0:18c"
*/
};
#endif


/*
static int add_bcast_addr(void *priv, void *addr)
{
	printf("Callback Called\n");
	union mavldata *ud;
	ud = addr;
	printf("Callback Called %s\n", ud->str);	
	
	char *s = (char *) addr;
	conf_bcast_addrs[conf_bcast_addrs_len] = strdup(s);
	if (conf_bcast_addrs[conf_bcast_addrs_len] != 0)
		conf_bcast_addrs_len++;
	return 1;
}
*/


/*
 * Initialize broadcast addresses (ipv4 only)
 */
int init_bcast_addrs()
{
	int rc;
	char str[100];
	struct ifaddrs *ifa0, *ifa;
	mavl_t t;
	mavliter_t it;
	
	if (conf_bcast_addrs)
		return 1;

	if (!conf_ipv4)
		return 1;

	/*t = mavl_create_ptr(); */
	t = mavl_create_ptr(mavl_cmpstr,mavl_freeptr);
	if (t==NULL)
		return 0;

	/* add the default broadast address */
	mavl_add_str(t, cw_strdup("255.255.255.255"));


	/* add all other local broadcast addresses */
	
	rc = getifaddrs(&ifa0);
	if (rc == -1)
		return 0;

	for (ifa = ifa0; ifa != 0; ifa = ifa->ifa_next) {
		struct sockaddr *sa;
		if (!(ifa->ifa_flags & IFF_BROADCAST))
			continue;


		if (!conf_use_loopback) {
			if ((ifa->ifa_flags & IFF_LOOPBACK))
				continue;
		}


		sa = ifa->ifa_addr;
		if (!sa)
			continue;
		if (sa->sa_family != AF_INET)
			continue;


		if (ifa->ifa_broadaddr) {
			sock_addrtostr(ifa->ifa_broadaddr, str, 100,1);
			*strchr(str, ':') = 0;
			mavl_add_str(t, cw_strdup(str));
		}

	}

	conf_bcast_addrs = malloc(t->count * sizeof(char *));

	mavliter_init(&it,t);
	mavliter_foreach(&it){
		char * d;
		d = mavliter_get_str(&it);
		conf_bcast_addrs[conf_bcast_addrs_len] = cw_strdup(d);
		if (conf_bcast_addrs[conf_bcast_addrs_len] != 0)
			conf_bcast_addrs_len++;

	}


	mavl_destroy(t);

	freeifaddrs(ifa0);

	return 1;
}

int init_mcast_groups()
{
	int ctr;
	int i;
	
	int n = 0;
	int n4 = 0, n6 = 0;

	if (conf_mcast_groups)
		return 1;
	
	if (conf_ipv4) {
		n4 = sizeof(conf_default_mcast_groups_ipv4) / sizeof(char *);
	}

	if (conf_ipv6) {
		n6 = sizeof(conf_default_mcast_groups_ipv6) / sizeof(char *);
	}

	n = n4 + n6;
	if (n == 0)
		return 1;

	conf_mcast_groups = malloc(sizeof(char *) * n);
	if (!conf_mcast_groups)
		return 0;
	memset(conf_mcast_groups, 0, n * sizeof(char *));

	ctr = 0;

	for (i = 0; i < n4; i++) {
		uint8_t *g = (uint8_t *) conf_default_mcast_groups_ipv4[i];
		conf_mcast_groups[ctr] =
		    (char *) cw_setstr((uint8_t **) & conf_mcast_groups[ctr], g,
				       strlen((char *) g));
		if (conf_mcast_groups[ctr])
			ctr++;
	}

	for (i = 0; i < n6; i++) {
		uint8_t *g = (uint8_t *) conf_default_mcast_groups_ipv6[i];
		conf_mcast_groups[ctr] =
		    (char *) cw_setstr((uint8_t **) & conf_mcast_groups[ctr], g,
				       strlen((char *) g));
		if (conf_mcast_groups[ctr])
			ctr++;
	}


	conf_mcast_groups_len = n;
	return 1;

}

static int conf_read_strings(cfg_t * cfg, char *name, char ***dst, int *len)
{

	int n, i;
	n = cfg_size(cfg, name);
	*len = n;
	if (n == 0)
		return 1;

	*dst = malloc(sizeof(char *) * n);
	if (!*dst)
		return 0;

	for (i = 0; i < n; i++) {
		char *str = cfg_getnstr(cfg, name, i);
		if (!((*dst)[i] = malloc(strlen(str) + 1))) {
			*len = 0;
			return 0;
		}

		strcpy((*dst)[i], str);
	}
	return 1;
}



static int conf_read_dbg_level(cfg_t * cfg)
{
	const char *name = "dbg";
	int n, i;
	n = cfg_size(cfg, name);


	for (i = 0; i < n; i++) {
		char *str = cfg_getnstr(cfg, name, i);
		int b = cw_strlist_get_id(cw_dbg_strings, str);


		cw_dbg_set_level(b, 1);

	}
	return 1;
}

struct cw_Mod ** conf_mods; 
char *conf_mods_dir = NULL;

/*
 * Read the module names from config file
 */ 
static int conf_read_mods(cfg_t *cfg){

	int n, i;
	n = cfg_size(cfg,CFG_ENTRY_MODS);
	
	conf_mods = malloc(sizeof(struct cw_Mod *)*(n+1));

	cw_dbg(DBG_INFO,"Mods directory: %s",conf_mods_dir);
	cw_mod_set_path(conf_mods_dir);

	for (i=0; i < n; i++){
		char *modname = cfg_getnstr(cfg, CFG_ENTRY_MODS, i);
		struct cw_Mod * mod = cw_mod_load(modname, actube_global_cfg, CW_ROLE_AC);
		
		if (!mod)
			return 0;
		cw_mod_add_to_list(mod);
	}
	return 1;
}


int conf_parse_listen_addr(const char *addrstr, char *saddr, char *port, int *proto)
{
	char *b;
	int c;
	int i, l;
	char *ctrlport;
	char *p = strchr(addrstr, '/');
	if (!p)
		*proto = AC_PROTO_CAPWAP;
	else {
		if (strcmp("capwap", p + 1) == 0)
			*proto = AC_PROTO_CAPWAP;
		else if (strcmp("lwapp", p + 1) == 0)
			*proto = AC_PROTO_LWAPP;
		else
			*proto = AC_PROTO_UNKNOWN;
	}


	switch (*proto) {
		case AC_PROTO_CAPWAP:
			ctrlport = CAPWAP_CONTROL_PORT_STR;
			break;
		case AC_PROTO_LWAPP:
			ctrlport = LWAPP_CONTROL_PORT_STR;
			break;
		default:
			ctrlport = "0";
	}



	c = 0;

	if (p)
		l = p - addrstr;
	else
		l = strlen(addrstr);

	for (i = 0; i < l; i++) {
		if (addrstr[i] == ':')
			c++;
	}

	/* no colon found, asume its ipv4 w/o port */
	if (c == 0) {
		strncpy(saddr, addrstr, l);
		saddr[l] = 0;
		strcpy(port, ctrlport);
		return 0;
	}

	/* one colon found, so we assume, its an ipv4 
	 * address with port */
	if (c == 1) {
		char *c = strchr(addrstr, ':');
		strncpy(saddr, addrstr, c - addrstr);
		saddr[c - addrstr] = 0;
		strncpy(port, c + 1, l - (c - addrstr));
		port[l - (c - addrstr) - 1] = 0;
		return 0;
	}

	/* we assume now, its ipv6 */
	b = strstr(addrstr, "]:");
	if (!b) {
		/* ippv6 w/o port */
		strncpy(saddr, addrstr, l);
		saddr[l] = 0;
		strcpy(port, ctrlport);
		return 0;
	}

	strncpy(saddr, addrstr + 1, b - addrstr);
	saddr[b - addrstr - 1] = 0;
	strncpy(port, b + 2, l - (b - addrstr) - 2);
	port[l - (b - addrstr) - 2] = 0;
	return 0;
}

static void errfunc(cfg_t *cfg, const char *fmt, va_list ap){

	if (cfg && cfg->filename && cfg->line)
		fprintf(stderr, "ERROR in %s:%d: ", 
			cfg->filename, cfg->line);
	else if (cfg && cfg->filename)
		fprintf(stderr, "ERROR in %s:", cfg->filename);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr,"\n");
}


int read_config(const char *filename)
{
	int i, n;

	cfg_opt_t opts[] = {
		CFG_STR_LIST("mods", "{}", CFGF_NONE),
		CFG_SIMPLE_STR("mods_dir", &conf_mods_dir),
		
		CFG_STR_LIST("dbg", "{}", CFGF_NONE),
		CFG_STR_LIST("listen", "{}", CFGF_NONE),
		CFG_STR_LIST("mcast_groups", "{}", CFGF_NONE),
		CFG_STR_LIST("broadcast_listen", "{}", CFGF_NONE),
		CFG_STR_LIST("ac_ips", "{}", CFGF_NONE),
		CFG_SIMPLE_STR("control_port", &conf_control_port),

		CFG_SIMPLE_BOOL("strict_capwap", &conf_strict_capwap),
		CFG_SIMPLE_BOOL("strict_headers", &conf_strict_headers),
		CFG_SIMPLE_BOOL("use_loopback", &conf_use_loopback),
/*//		CFG_SIMPLE_STR("capwap_mode", &conf_capwap_mode_str),*/


#ifdef WITH_LWAPP
		CFG_SIMPLE_STR("lw_control_port", &conf_lw_control_port),
		CFG_SIMPLE_BOOL("lwapp", &conf_lwapp),
#endif

		CFG_SIMPLE_INT("max_wtps", &conf_max_wtps),
		CFG_SIMPLE_INT("debug_level", &conf_debug_level),


		CFG_SIMPLE_INT("vendor_id", &conf_vendor_id),
		CFG_SIMPLE_STR("ac_id", &conf_acid),
		CFG_SIMPLE_STR("ac_name", &conf_acname),
		CFG_SIMPLE_STR("hardware_version", &conf_hardware_version),
		CFG_SIMPLE_STR("software_version", &conf_software_version),

		CFG_SIMPLE_STR("cisco_hardware_version", &conf_cisco_hardware_version),
		CFG_SIMPLE_STR("cisco_software_version", &conf_cisco_software_version),



		CFG_SIMPLE_STR("ssl_cert", &conf_sslcertfilename),
		CFG_SIMPLE_STR("ssl_key", &conf_sslkeyfilename),
		CFG_SIMPLE_STR("ssl_key_pass", &conf_sslkeypass),
		CFG_SIMPLE_STR("ssl_cipher", &conf_sslcipher),
		CFG_SIMPLE_STR("dtls_psk", &conf_dtls_psk),

		CFG_SIMPLE_BOOL("dtls_verify_peer", &conf_dtls_verify_peer),
		CFG_SIMPLE_BOOL("ipv4", &conf_ipv4),
		CFG_SIMPLE_BOOL("ipv6", &conf_ipv6),
		CFG_SIMPLE_STR("db_file", &conf_db_file),
		CFG_SIMPLE_STR("image_dir", &conf_image_dir),

		CFG_END()
	};
	cfg_t *cfg;

	conf_mods_dir=cw_strdup("");

	if (!init_control_port())
		return 0;

	
	cfg = cfg_init(opts, CFGF_NONE);
	cfg_set_error_function(cfg, errfunc);

	if (cfg_parse(cfg, filename))
		return 0;
	

	/* read debug options */
	conf_read_dbg_level(cfg);

	/* read the listen addresses */
	conf_read_strings(cfg, "listen", &conf_listen_addrs, &conf_listen_addrs_len);

	/* read multi cast groups */
	conf_read_strings(cfg, "mcast_groups", &conf_mcast_groups,
			  &conf_mcast_groups_len);

	/* read ipv4 broadcast addresses */
	conf_read_strings(cfg, "broadcast_listen", &conf_bcast_addrs, &conf_bcast_addrs_len);


	/* read ac_ips */
	n = cfg_size(cfg, "ac_ips");
	if (!(conf_ac_ips = malloc(sizeof(struct sockaddr) * n)))
		return 0;

	conf_ac_ips_len = n;
	for (i = 0; i < n; i++) {
		struct sockaddr sa;
		char *str = cfg_getnstr(cfg, "ac_ips", i);
		if (sock_strtoaddr(cfg_getnstr(cfg, "ac_ips", i), &sa))
			conf_ac_ips[i] = sa;
		else {
			perror(str);
		}
	}


	if (!conf_read_mods(cfg)){
		cfg_free(cfg);
		return 0;
	}


	cfg_free(cfg);

	if (!init_acid())
		return 0;

	if (!init_acname())
		return 0;

/*	if (!init_version())
		return 0;
*/

	if (!init_vendor_id())
		return 0;

	if (!init_dtls())
		return 0;

	if (!conf_sslcipher)
		conf_sslcipher = CAPWAP_CIPHER;

	if (!conf_image_dir)
		conf_image_dir = CONF_DEFAULT_IMAGE_DIR;



	init_listen_addrs();
	init_mcast_groups();
	init_bcast_addrs();


	return 1;
}

void free_config()
{

}
