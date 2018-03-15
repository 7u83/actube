#include "ktv.h"
#include "keys.h"
#include "log.h"
#include "cw.h"

int cw_write_descriptor_subelem (uint8_t *dst, mavl_t ktvstore,
                                 int subelem_id, const char * parent_key)
{
	char key[256];
	cw_KTV_t * vendor, *version ;
	uint8_t *d;
	
	
	/*        d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	*/
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VENDOR);
	vendor = cw_ktv_get (ktvstore, key, CW_TYPE_DWORD);
	
	if (vendor == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, not value found.", key);
		return 0;
	}
	
	
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VERSION);
	version = cw_ktv_get (ktvstore, key, CW_TYPE_BSTR16);
	
	if (version == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, not value found.", key);
		return 0;
	}
	
	d = dst;
	
	/* put vendor */
	d += vendor->type->put (vendor, d);
	
	/* put version */
	
	d += cw_put_dword (d, (subelem_id << 16) | version->type->len(version));
	d += version->type->put(version,d);
	
	return d-dst;
}
