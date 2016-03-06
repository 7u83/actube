
#include "cw/capwap_items.h"
#include "cw/format.h"

#include "wtp.h"
#include "wtp_conf.h"
#include "wtp_interface.h"


int setup_conf(struct conn *  conn)
{
	conn->base_rmac = get_base_rmac();

	/* Configure WTP Name */
	struct mbag_item * n = mbag_get(conn->config,CW_ITEM_WTP_NAME);
	if (!n) {
		/* If no name was read from cfg, build a name from
		 * base rmac */
		char name[32];
		strcpy(name,WTP_DEFAULT_NAME_PREFIX);
		int l = cw_format_hexu(name+strlen(WTP_DEFAULT_NAME_PREFIX),
				bstr_data(conn->base_rmac),bstr_len(conn->base_rmac));

		name[l+strlen(WTP_DEFAULT_NAME_PREFIX)]=0;

		mbag_set_str(conn->config,CW_ITEM_WTP_NAME,name);
	}

	n = mbag_get(conn->config,CW_ITEM_LOCATION_DATA);
	if (!n) {
		mbag_set_str(conn->config,CW_ITEM_LOCATION_DATA,WTP_DEFAULT_LOCATION_DATA);

	}

	n = mbag_get(conn->config,CW_ITEM_WTP_HARDWARE_VERSION);
	if (!n) {
		mbag_set_bstrv(conn->config, CW_ITEM_WTP_HARDWARE_VERSION,
			CONF_DEFAULT_HARDWARE_VENDOR_ID,
			(uint8_t*)CONF_DEFAULT_HARDWARE_VERSION,
			strlen(CONF_DEFAULT_HARDWARE_VERSION));
	}


	n = mbag_get(conn->config,CW_ITEM_WTP_BOOTLOADER_VERSION);
	if (!n) {
		mbag_set_bstrv(conn->config, CW_ITEM_WTP_BOOTLOADER_VERSION,
			CONF_DEFAULT_BOOTLOADER_VENDOR_ID,
			(uint8_t*)CONF_DEFAULT_BOOTLOADER_VERSION,
			strlen(CONF_DEFAULT_BOOTLOADER_VERSION));
	}

	n = mbag_get(conn->config,CW_ITEM_WTP_SOFTWARE_VERSION);
	if (!n) {
		mbag_set_bstrv(conn->config, CW_ITEM_WTP_SOFTWARE_VERSION,
			CONF_DEFAULT_SOFTWARE_VENDOR_ID,
			(uint8_t*)CONF_DEFAULT_SOFTWARE_VERSION,
			strlen(CONF_DEFAULT_SOFTWARE_VERSION));
	}



	mbag_set_byte(conn->config,CW_ITEM_ECN_SUPPORT,get_ecn_support());


	return 0;
}
