
#include "cw/capwap_items.h"
#include "cw/format.h"

#include "wtp.h"
#include "wtp_conf.h"
#include "wtp_interface.h"

#include "cw/vendors.h"


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
		int l = format_hexu(name+strlen(WTP_DEFAULT_NAME_PREFIX),
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

	n = mbag_get(conn->config,CW_ITEM_WTP_REBOOT_STATISTICS);
	if (!n){
		mbag_t rs =mbag_create();
		mbag_set_mbag(conn->config, CW_ITEM_WTP_REBOOT_STATISTICS,rs);


		mbag_set_dword(rs,CW_ITEM_REBOOT_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_AC_INITIATED_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_LINK_FAILURE_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_SW_FAILURE_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_HW_FAILURE_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_OTHER_FAILURE_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_UNKNOWN_FAILURE_COUNT,0);
		mbag_set_dword(rs,CW_ITEM_REBOOT_LAST_FAILURE_TYPE, CW_REBOOT_TYPE_NOT_SUPPORTED);
	}

	n = mbag_get(conn->config,CW_ITEM_STATISTICS_TIMER);
	if (!n) {
		mbag_set_word(conn->config, CW_ITEM_STATISTICS_TIMER, 120);
	}




	mbag_t board_data=mbag_get_mbag_c(conn->config,CW_ITEM_WTP_BOARD_DATA,mbag_create);

	printf("We have bd %p\n",board_data);

//	mbag_set_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, board_data);


	uint32_t vendor = mbag_get_dword(board_data,CW_ITEM_WTP_BOARD_VENDOR,conf_vendor_id);
	mbag_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR, vendor);


	mbag_set_bstrn(board_data, CW_ITEM_WTP_BOARD_MACADDRESS, 
			 conf_macaddress, conf_macaddress_len);

//	mbag_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_MODELNO, bstr_data(conf_model_no),
//			 bstr_len(conf_model_no));







	mbag_set_byte(conn->config,CW_ITEM_CAPWAP_TRANSPORT_PROTOCOL,0);
















	return 0;
}
