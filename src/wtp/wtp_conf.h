#ifndef __WTP_CONF_H
#define __WTP_CONF_H

#include <sys/socket.h>

#include "cw/capwap.h"
#include "cw/bstr.h"




extern int read_config(const char * filename);
extern char * conf_wtpname;
extern char * conf_dtls_psk;
extern char * conf_sslkeyfilename;
extern char * conf_sslcertfilename;
extern char * conf_sslkeypass;


extern uint32_t conf_vendor_id;
extern uint32_t * conf_bootloader_vendor_id;
extern uint8_t * conf_bootloader_version;
extern uint32_t * conf_software_vendor_id;
extern uint8_t * conf_software_version;
extern uint32_t * conf_hardware_vendor_id;
extern uint8_t * conf_hardware_version;

extern uint8_t * conf_model_no;
extern uint8_t * conf_serial_no;


extern bstr_t conf_board_id;
extern bstr_t conf_board_revision;


extern struct sockaddr_storage * conf_preffered_ac_sa;
extern char * conf_primary_if;
extern char * conf_ip;

extern char ** conf_ac_list;

extern int conf_ac_list_len;
extern char * conf_control_port;

extern uint8_t conf_macaddress[12];
extern uint8_t conf_macaddress_len;


#ifndef CONF_DEFAULT_VENDOR_ID
	#define CONF_DEFAULT_VENDOR_ID CW_VENDOR_ID_FSF
#endif

#ifndef CONF_DEFAULT_HARDWARE_VENDOR_ID
	#define CONF_DEFAULT_HARDWARE_VENDOR_ID CW_VENDOR_ID_FSF
#endif

#ifndef CONF_DEFAULT_SOFTWARE_VENDOR_ID
	#define CONF_DEFAULT_SOFTWARE_VENDOR_ID CW_VENDOR_ID_FSF
#endif

#ifndef CONF_DEFAULT_BOOTLOADER_VENDOR_ID
	#define CONF_DEFAULT_BOOTLOADER_VENDOR_ID CW_VENDOR_ID_FSF
#endif

#ifndef CONF_DEFAULT_BOOTLOADER_VERSION
	#define CONF_DEFAULT_BOOTLOADER_VERSION "W-FAT-BL-0.1"
#endif

#ifndef CONF_DEFAULT_MODEL_NO
	#define CONF_DEFAULT_MODEL_NO "W-FAT"
#endif

#ifndef CONF_DEFAULT_SERIAL_NO
	#define CONF_DEFAULT_SERIAL_NO "0000"
#endif

#ifndef CONF_DEFAULT_SOFTWARE_VERSION
	#define CONF_DEFAULT_SOFTWARE_VERSION "0.1"
#endif

#ifndef CONF_DEFAULT_HARDWARE_VERSION
	#define CONF_DEFAULT_HARDWARE_VERSION "0.1"
#endif


#ifndef CONF_DEFAULT_BOARD_ID
	#define CONF_DEFAULT_BOARD_ID "00"
#endif

#ifndef CONF_DEFAULT_BOARD_REVISION
	#define CONF_DEFAULT_BOARD_REVISION "00"
#endif


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

#define CONF_DEFAULT_DEBUG_LEVEL 0

#define CONF_DEFAULT_MTU_DISCOVERY 1

extern long conf_max_discovery_interval;
extern long conf_discovery_interval;
extern long conf_silent_interval;
extern long conf_max_discoveries;
extern char * conf_dtls_cipher;
extern long conf_echo_interval;
extern long conf_max_retransmit;
extern long conf_retransmit_interval;

//extern long conf_dbg_level;
extern int conf_mtu_discovery;
extern int conf_mtu;




typedef struct {
	const char *name;
	long *value;	
}LONGSTRS;
extern LONGSTRS conf_timer_cfgstrs[];


int wtpconf_preinit();
int wtpconf_init();


#define WTP_DEFAULT_NAME_PREFIX "WFAT-"
#define WTP_DEFAULT_LOCATION_DATA "Superposition"

#endif
