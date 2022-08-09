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
#include "cw/cfg.h"

#include "cw/mavltypes.h"

/*uint8_t conf_macaddress[12];
uint8_t conf_macaddress_len = 0;
*/

long conf_strict_capwap = 1;
long conf_strict_headers = 0;
/*int conf_capwap_mode = CW_MODE_CAPWAP;*/



char *conf_acid = NULL;

char *conf_primary_if = NULL;

long conf_max_wtps = CONF_DEFAULT_MAXWTPS;
char *conf_logfilename = CONF_DEFAULT_LOGFILENAME;
struct sockaddr_storage *conf_salist = NULL;



char **conf_mcast_groups = 0;
int conf_mcast_groups_len = 0;


struct sockaddr_storage *conf_bsalist = NULL;

int conf_salist_len = 0;
int conf_bsalist_len = 0;

struct sockaddr *conf_ac_ips;
int conf_ac_ips_len;

char *conf_sslcertfilename = NULL;
char *conf_sslkeyfilename = NULL;
char *conf_sslkeypass = NULL;
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

static int init_ac_name(cw_Cfg_t * cfg)
{
	char *s;
	char *primary_if=NULL;
	char ac_name[CAPWAP_MAX_AC_NAME_LEN+1];
	uint8_t macaddress[128];
	uint8_t macaddress_len;
	char acid[128];
	int i;


	s= cw_cfg_get(cfg,"capwap/ac-name",NULL);
	if (s!=NULL)
		return 1;

//	primary_if = sock_get_primary_if(AF_INET6);
	if (!primary_if)
		primary_if = sock_get_primary_if(AF_INET);

	acid[0]=0;

	if (primary_if) {
		macaddress_len=8;
		if (!sock_getifhwaddr(primary_if, macaddress, &macaddress_len)) {
			cw_dbg(DBG_INFO, "Fatal: Unable to detect link layer address for %s\n",
			       primary_if);
		}
		else{
			s=acid;
			for (i = 0; i < macaddress_len; i++) {
				s += sprintf(s, "%02X", macaddress[i]);
			}

		}
		free(primary_if);
	}

	sprintf(ac_name,"actube%s",acid);
	cw_cfg_set(cfg,"capwap/ac-name",ac_name);
	return 1;
}

#include "../mod/modload.h"




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

static int init_listen_addrs(cw_Cfg_t * cfg)
{
	char key[CW_CFG_MAX_KEY_LEN];

	struct ifaddrs *ifap, *ifa;
	int rc;
	int ctr;
	struct cw_Cfg_iter cfi;



        cw_cfg_iter_init(cfg, &cfi, "actube/listen");
	if (cw_cfg_iter_next(&cfi,NULL) != NULL)
		return 0;

	rc = getifaddrs(&ifap);
	if (rc == -1)
		return 0;

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


			sprintf(key,"actube/listen.%d",ctr++);
			cw_cfg_set(cfg,key,str);
		}
		if (ifa->ifa_addr->sa_family == AF_INET6 && conf_ipv6) {
			sock_addrtostr(ifa->ifa_addr, str, 100,0);

			if (strncmp(str, "fe80:", 5) == 0) {
				strcat(str, "%");
				strcat(str, ifa->ifa_name);
			}
			sprintf(key,"actube/listen.%d",ctr++);
			cw_cfg_set(cfg,key,str);
		}
	}
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
int init_bcast_addrs(cw_Cfg_t *cfg)
{
	int rc;
	char str[100];
	char key[100];
	struct ifaddrs *ifa0, *ifa;
	mavl_t t;
	mavliter_t it;
	struct cw_Cfg_iter cfi;
	int i;

//printf("BCAST INIT\n");
        cw_cfg_iter_init(cfg, &cfi, "actube/bcast");
	if (cw_cfg_iter_next(&cfi,NULL) != NULL)
		return 0;

	
	if (!cw_cfg_get_bool(cfg,"actube/ipv4", "true"))
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
			char *s,*sr;
			sock_addrtostr(ifa->ifa_broadaddr, str, 100,1);
			*strchr(str, ':') = 0;


			s = cw_strdup(str);
			sr = mavl_add_str(t, s);


			printf("BCAST = %p --- %p: %s\n",str,s,str);
		}

	}

//	conf_bcast_addrs = malloc(t->count * sizeof(char *));

	i=0;
	mavliter_init(&it,t);
	mavliter_foreach(&it){
		char * d;
		d = mavliter_get_str(&it);
	//	conf_bcast_addrs[conf_bcast_addrs_len] = cw_strdup(d);
	//	if (conf_bcast_addrs[conf_bcast_addrs_len] != 0)
	//		conf_bcast_addrs_len++;

		sprintf(key,"actube/bcast.%d",i++);
		cw_cfg_set(cfg,key,d);


	}


	mavl_destroy(t);

	freeifaddrs(ifa0);

	return 1;
}

int init_mcast_groups(cw_Cfg_t * cfg)
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

/*
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

*/

/*
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
*/

struct cw_Mod ** conf_mods; 
char *conf_mods_dir = NULL;

/*
 * Read the module names from config file
 */ 
static int init_mods(cw_Cfg_t *cfg){

	int n, i;
	const char * modname;

	struct cw_Cfg_iter cfi;
        cw_cfg_iter_init(cfg, &cfi, "actube/mod");
	if (cw_cfg_iter_next(&cfi,NULL) == NULL){
		cw_cfg_set(cfg,"actube/mod.0","capwap");
		cw_cfg_set(cfg,"actube/mod.1","capwap80211");
	}

//	cw_dbg(DBG_INFO,"Mods directory: %s",conf_mods_dir);
//	cw_mod_set_path(conf_mods_dir);

        cw_cfg_iter_init(cfg, &cfi, "actube/mod");
printf("iter mods\n");	
        for (i=0; (modname = cw_cfg_iter_next(&cfi, NULL)) != NULL; i++) {

		printf("init mod name: %s\n",modname);

		struct cw_Mod * mod = cw_mod_load(modname, cfg, CW_ROLE_AC);
		
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

void free_config()
{

}




void ac_conf_init(cw_Cfg_t *cfg)
{
	printf("ac conf\n");
	init_listen_addrs(cfg);
	init_bcast_addrs(cfg);
	init_ac_name(cfg);
	init_mods(cfg);
}
