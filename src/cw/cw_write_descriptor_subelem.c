#include "val.h"
#include "keys.h"
#include "log.h"
#include "cw.h"
#include "dbg.h"
#include "cfg.h"

int cw_write_descriptor_subelem (uint8_t *dst, cw_Cfg_t * cfg,
                                 int subelem_id, const char * parent_key )
{
	char key[256];
	uint32_t vendor;
	bstr16_t version;
	const char *vendor_s;

	uint8_t *d;

	/*        d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	*/
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VENDOR);
	vendor_s = cw_cfg_get (cfg, key, NULL);
	
	if (vendor_s == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value of type Dword found.", key);
		return 0;
	}

	vendor = atoi(vendor_s);	
	
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VERSION);
	version = cw_cfg_get_bstr16 (cfg, key, NULL);
	
	if (version == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value of type Bstr16 found.", key);
		return 0;
	}
	
	d = dst;
	
	/* put vendor */
	d += cw_put_dword(d, vendor); //->type->put (vendor, d);
	
	/* put version */
	
	d += cw_put_dword (d, (subelem_id << 16) | bstr16_len(version));
	d += cw_put_bstr16(d, version);

	free(version);
	
	return d-dst;
}
