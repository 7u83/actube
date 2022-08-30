#include "val.h"
#include "keys.h"
#include "log.h"
#include "cw.h"
#include "dbg.h"
#include "cfg.h"

int cw_write_descriptor_subelem (uint8_t *dst, cw_Cfg_t ** cfg_list,
                                 int subelem_id, const char * parent_key )
{
	char key[CW_CFG_MAX_KEY_LEN];
	uint32_t vendor;
	bstr16_t version;
	const char *vendor_s;

	uint8_t *d;

	sprintf (key, "%s/%s", parent_key, CW_SKEY_VENDOR);
	vendor_s = cw_cfg_get_l (cfg_list, key, NULL);
	if (vendor_s == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value found. Setting zero.", key);
		vendor_s = "0";
	}

	vendor = atoi(vendor_s);	
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VERSION);
	version = cw_cfg_get_bstr16_l(cfg_list,key,NULL);
	if (version == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value found. Setting zero.", key);
		version = bstr16_create_from_str("0.0.0.0");
	}
	
	d = dst;
	
	/* put vendor */
	d += cw_put_dword(d, vendor); 
	
	/* put version */
	d += cw_put_dword (d, (subelem_id << 16) | bstr16_len(version));
	d += cw_put_bstr16(d, version);

	free(version);
	
	return d-dst;
}
