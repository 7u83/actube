#ifndef __WTP_CONF_H
#define __WTP_CONF_H

#include <sys/socket.h>

#include "capwap/capwap.h"

extern int read_config(const char * filename);
extern char * conf_wtpname;
extern char * conf_dtls_psk;
extern struct sockaddr_storage * conf_preffered_ac_sa;
extern char * conf_primary_if;

extern char ** conf_ac_list;

extern int conf_ac_list_len;
extern char * conf_control_port;

extern uint8_t conf_macaddress[12];
extern uint8_t conf_macaddress_len;



#ifndef CONF_DEFAULT_MAX_DISCOVERY_INTERVAL
	#define CONF_DEFAULT_MAX_DISCOVERY_INTERVAL CAPWAP_MAX_DISCOVERY_INTERVAL
#endif

#ifndef CONF_DEFAULT_SILENT_INTERVAL
	#define CONF_DEFAULT_SILENT_INTERVAL CAPWAP_SILENT_INTERVAL
#endif

#ifndef CONF_DEFAULT_DISCOVERY_INTERVAL
	#define CONF_DEFAULT_DISCOVERY_INTERVAL CAPWAP_DISCOVERY_INTERVAL
#endif

#ifndef CONF_DEFAULT_MAX_DISCOVERIES
	#define CONF_DEFAULT_MAX_DISCOVERIES CAPWAP_MAX_DISCOVERIES
#endif

#ifndef CONF_DEFAULT_DTLS_CIPHER
	#define CONF_DEFAULT_DTLS_CIPHER CAPWAP_CIPHER
#endif

#define CONF_DEFAULT_ECHO_INTERVAL CAPWAP_ECHO_INTERVAL
#define CONF_DEFAULT_MAX_RETRANSMIT CAPWAP_MAX_RETRANSMIT
#define CONF_DEFAULT_RETRANSMIT_INTERVAL CAPWAP_RETRANSMIT_INTERVAL

#define CONF_DEFAULT_DEBUG_LEVEL -1


extern long conf_max_discovery_interval;
extern long conf_discovery_interval;
extern long conf_silent_interval;
extern long conf_max_discoveries;
extern char * conf_dtls_cipher;
extern long conf_echo_interval;
extern long conf_max_retransmit;
extern long conf_retransmit_interval;

extern long conf_debug_level;

#endif
