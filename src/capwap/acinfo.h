/**
 *@file
 *@brief acinfo definitions.
 */

#ifndef __ACINFO_H
#define __ACINFO_H


#include <stdint.h>
#include <sys/socket.h>

#include "avltree.h"

#include "radioinfo.h"


/**
 * Structure to hold info about an AC 
 */
struct XXXac_info{
	int stations;		
	int limit;
	int active_wtps;
	int max_wtps;
	uint8_t * ac_name;
	int ac_name_len;
	int security;
	int rmac;
	int dtls_policy;
	int vendor_id;

	/** Hardware version to use in capwap-mode */
	bstr_t hardware_version;
	/** Software version to use in capwap-mode */
	bstr_t software_version;

	/** Hardware version to use if in cisco-mode */
	bstr_t cisco_hardware_version; 
	/** Software version to use if in cisco-mode */
	bstr_t cisco_software_version;



	struct sockaddr * salist;
	int salist_len;

	//ACIPLIST * aciplist;
	uint8_t ecn_support;

	struct sockaddr_storage local_ip;

	/* wireless bindings */
	

	struct radioinfo radioinfos[32];

	uint32_t result_code;

};

typedef struct ac_info ACINFO;

extern int acinfo_readelem_ac_name(struct ac_info * acinfo,int type, uint8_t *msgelem, int len);
extern int acinfo_readelem_ac_descriptor(struct ac_info * acinfo,int type, uint8_t *msgelem, int len);
extern int acinfo_readelem_ctrl_ip_addr(struct ac_info * acinfo, int type, uint8_t*msgelem,int len);
extern int acinfo_print(char *str,const struct ac_info *acinfo);

#define acinfo_readelem_ecn_support(acinfo,type,msg,len) cw_readelem_ecn_support(&acinfo->ecn_support,type,msg,len)
#define acinfo_readelem_cw_local_ip_addr(acinfo,type,msg,len) cw_readelem_cw_local_ip_addr(&acinfo->local_ip,type,msg,len)
#define acinfo_readelem_ac_name(acinfo,type,msg,len) cw_readelem_ac_name(&acinfo->ac_name,type,msg,len)

#endif

