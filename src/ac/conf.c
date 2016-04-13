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

#include "conf.h"

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/cw_util.h"
#include "cw/action.h"

#include "ac.h"

uint8_t conf_macaddress[12];
uint8_t conf_macaddress_len = 0;


long conf_strict_capwap = 1;
long conf_strict_headers = 0;
char *conf_capwap_mode_str = NULL;
int conf_capwap_mode = CW_MODE_CAPWAP;



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

//char * conf_ac_hardware_version=NULL;
//char * conf_ac_software_version=NULL;
int conf_security = 0;
long conf_vendor_id = CONF_DEFAULT_VENDOR_ID;


bstr_t conf_hardware_version = NULL;
bstr_t conf_software_version = NULL;

bstr_t conf_cisco_hardware_version = NULL;
bstr_t conf_cisco_software_version = NULL;


//int conf_hardware_version_len=0;

//int conf_software_version_len=0;


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


struct mod_ac * conf_mods[10];

static int init_mods()
{

	conf_mods[0]=modload_ac("cisco");
	conf_mods[1]=modload_ac("fortinet");
	conf_mods[2]=modload_ac("zyxel");
	conf_mods[3]=modload_ac("cipwap");
	conf_mods[4]=modload_ac("capwap");
	conf_mods[5]=modload_ac("capwap80211");
	conf_mods[6]=NULL;




	int i;
	for(i=0; conf_mods[i]; i++){
		if (conf_mods[i]->init){
			conf_mods[i]->init();
		}
	}

	return 0;
}

static int init_acid()
{

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

	int i;


	conf_acid = malloc(2 * conf_macaddress_len + 1);
	char *s = conf_acid;

	for (i = 0; i < conf_macaddress_len; i++) {
		s += sprintf(s, "%02X", conf_macaddress[i]);
	}
	return 1;

}

static int init_dtls()
{
	if (conf_dtls_psk != NULL) {
//              conf_security=CW_SECURITY_FLAGS_S;
	}

	return 1;
}


static int init_vendor_id()
{
	return 1;
}

/*
static int convert_version_string(char * si[], int *l)
{
	char * s = *si;	

	if (*l<=1)
		return 1;

	if (s[0]!='.'){
		return 1;
	}

	if (*l<=2)
		return 1;

	if (s[1]=='.'){
		char * ns = malloc(*l-1);
		strcpy (ns,s+1);
		free(*si);
		*si=ns;
		*l-=1;
		return 1;
	}

	if (s[1]=='x'){
		char * ns=0;
		int len=0;
		int ch,cl;
		char *ss = s+2;
		int rc ;
		do {
			rc = sscanf(ss,"%01X",&ch);
			if (rc!=1)
				break;
			ss++;
			rc = sscanf(ss,"%01X",&cl);
			if (!rc)
				cl=0;
			ss++;
			int c=(ch<<4) | cl;
			
			len++;
			ns = realloc(ns,len);
			ns[len-1]=c;


		}while (rc==1);
		free(*si);
		*si=ns;
		return 1;

	}

	if (strcmp(s,".reflect")==0){
		free(*si);
		*si=0;
		*l=0;
		return 1;
	}

	return 1;
}

*/


static int init_version()
{

	/* Init hardware version */
	if (!conf_hardware_version) {
		struct utsname u;
		int rc = uname(&u);
		if (rc < 0)
			conf_hardware_version =
			    (bstr_t) strdup(CONF_DEFAULT_HARDWARE_VERSION);
		else {
			char str[265];
			sprintf(str, "%s / %s %s", u.machine, u.sysname, u.release);
			conf_hardware_version = (bstr_t) strdup(str);
		}

	}
	bstr_replace(&conf_hardware_version,
		     bstr_create_from_cfgstr((char *) conf_hardware_version));

	/* software version */
	if (!conf_software_version)
		conf_software_version = (bstr_t) strdup(CONF_DEFAULT_SOFTWARE_VERSION);
	bstr_replace(&conf_software_version,
		     bstr_create_from_cfgstr((char *) conf_software_version));


	/* Cisco hardware version */
	if (!conf_cisco_hardware_version)
		conf_cisco_hardware_version =
		    (bstr_t) strdup(CONF_DEFAULT_CISCO_HARDWARE_VERSION);
	bstr_replace(&conf_cisco_hardware_version,
		     bstr_create_from_cfgstr((char *) conf_cisco_hardware_version));

	/* Cisco software version */
/*
	if (!conf_cisco_software_version)
		conf_cisco_software_version=(bstr_t)strdup(CONF_DEFAULT_CISCO_SOFTWARE_VERSION);
	bstr_replace(&conf_cisco_software_version,bstr_create_from_cfgstr((char*)conf_cisco_software_version));
*/



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
	if (conf_listen_addrs != 0)
		return 1;

	struct ifaddrs *ifap, *ifa;

	int rc = getifaddrs(&ifap);
	if (rc == -1)
		return 0;

	/* count the addresses */
	int ctr = 0;
	for (ifa = ifap; ifa != 0; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET && conf_ipv4)
			ctr++;
#ifdef WITH_IPV6
		if (ifa->ifa_addr->sa_family == AF_INET6 && conf_ipv6)
			ctr++;
#endif
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
//			printf("The converter has %s\n",str);
//			*strchr(str, ':') = 0;
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
/*	"ff01:0:0:0:0:0:0:18c",
	"ff02:0:0:0:0:0:0:18c%em0",
	"ff03:0:0:0:0:0:0:18c",
	"ff04:0:0:0:0:0:0:18c",
	"ff05:0:0:0:0:0:0:18c",
	"ff06:0:0:0:0:0:0:18c"
*/
};
#endif

//#include "avltree"
#include "cw/stravltree.h"



static int add_bcast_addr(void *priv, void *addr)
{
	char *s = (char *) addr;
	conf_bcast_addrs[conf_bcast_addrs_len] = strdup(s);
	if (conf_bcast_addrs[conf_bcast_addrs_len] != 0)
		conf_bcast_addrs_len++;
	return 1;
}

/*
 * Initialize broadcast addresses (ipv4 only)
 */
int init_bcast_addrs()
{
	if (conf_bcast_addrs)
		return 1;

	if (!conf_ipv4)
		return 1;

	struct avltree *t = stravltree_create();
	if (!t)
		return 0;

	/* add the default broadast address */
	stravltree_add(t, "255.255.255.255");


	/* add all other local broadcast addresses */
	struct ifaddrs *ifa0, *ifa;
	int rc = getifaddrs(&ifa0);
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

		char str[100];
		if (ifa->ifa_broadaddr) {
			sock_addrtostr(ifa->ifa_broadaddr, str, 100,1);
			*strchr(str, ':') = 0;
			stravltree_add(t, str);
		}

	}

	conf_bcast_addrs = malloc(t->count * sizeof(char *));

	stravltree_foreach_asc(t, add_bcast_addr, 0);
	stravltree_destroy(t);

	freeifaddrs(ifa0);

	return 1;
}

int init_mcast_groups()
{
	if (conf_mcast_groups)
		return 1;

	int n = 0;
	int n4 = 0, n6 = 0;
	if (conf_ipv4) {
		n4 = sizeof(conf_default_mcast_groups_ipv4) / sizeof(char *);
	}
#ifdef WITH_IPV6
	if (conf_ipv6) {
		n6 = sizeof(conf_default_mcast_groups_ipv6) / sizeof(char *);
	}
#endif
	n = n4 + n6;
	if (n == 0)
		return 1;

	conf_mcast_groups = malloc(sizeof(char *) * n);
	if (!conf_mcast_groups)
		return 0;
	memset(conf_mcast_groups, 0, n * sizeof(char *));

	int ctr = 0;
	int i;
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


/*
struct conf_dbg_level_names{
	const char *name;
	int level;
};

*/


static int conf_read_dbg_level(cfg_t * cfg)
{
	const char *name = "dbg";
	int n, i;
	n = cfg_size(cfg, name);


	for (i = 0; i < n; i++) {
		char *str = cfg_getnstr(cfg, name, i);
//int u = cw_log_str2dbglevel(str);
		int b = cw_strlist_get_id(cw_dbg_strings, str);	//cw_log_str2dbglevel(str);

		cw_dbg_set_level(b, 1);

	}
	return 1;
}

void conf_init_capwap_mode()
{
	if (conf_capwap_mode_str == NULL)
		return;

	if (0 == strcmp(conf_capwap_mode_str, "cipwap")) {
		conf_capwap_mode = CW_MODE_CIPWAP;
	}

	if (0 == strcmp(conf_capwap_mode_str, "capwap")) {
		conf_capwap_mode = CW_MODE_CAPWAP;
	}


}



int conf_parse_listen_addr(const char *addrstr, char *saddr, char *port, int *proto)
{
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

	char *ctrlport;
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


	int i, l;
	int c = 0;

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
	char *b = strstr(addrstr, "]:");
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




int read_config(const char *filename)
{
	int i, n;

	if (!init_control_port())
		return 0;







	cfg_opt_t opts[] = {
		CFG_STR_LIST("dbg", "{}", CFGF_NONE),
		CFG_STR_LIST("listen", "{}", CFGF_NONE),
		CFG_STR_LIST("mcast_groups", "{}", CFGF_NONE),
		CFG_STR_LIST("broadcast_listen", "{}", CFGF_NONE),
		CFG_STR_LIST("ac_ips", "{}", CFGF_NONE),
		CFG_SIMPLE_STR("control_port", &conf_control_port),

		CFG_SIMPLE_BOOL("strict_capwap", &conf_strict_capwap),
		CFG_SIMPLE_BOOL("strict_headers", &conf_strict_headers),
		CFG_SIMPLE_BOOL("use_loopback", &conf_use_loopback),
		CFG_SIMPLE_STR("capwap_mode", &conf_capwap_mode_str),


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
		CFG_SIMPLE_STR("db_file", conf_db_file),
		CFG_SIMPLE_STR("image_dir", &conf_image_dir),

		CFG_END()
	};
	cfg_t *cfg;
	cfg = cfg_init(opts, 0);

	cfg_parse(cfg, filename);

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


	cfg_free(cfg);

	if (!init_acid())
		return 0;

	if (!init_acname())
		return 0;

	if (!init_version())
		return 0;


	if (!init_vendor_id())
		return 0;

	if (!init_dtls())
		return 0;

	if (!conf_sslcipher)
		conf_sslcipher = CAPWAP_CIPHER;

	if (!conf_image_dir)
		conf_image_dir = CONF_DEFAULT_IMAGE_DIR;

	init_mods();

	conf_init_capwap_mode();


	init_listen_addrs();
	init_mcast_groups();
	init_bcast_addrs();


	return 1;
}

void free_config()
{

}
