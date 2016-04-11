
#include "cw/action.h"
#include "cw/dbg.h"
#include "cw/log.h"
#include "cw/mbag.h"
#include "cw/cw.h"
#include "cw/capwap_items.h"

#include "cisco_items.h"

int cw_cisco_get_wlan_legacy(mbag_t wlan, uint8_t *data, int len)
{
	mbag_set_word(wlan,"enc_capab", cw_get_word(data+1));

	int wlan_id=cw_get_word(data+3);
	mbag_set_word(wlan,"wlan_id",wlan_id);

	mbag_set_dword(wlan,"enc_policy",cw_get_dword(data+5));
	mbag_set_bstr16n(wlan,"key",data+9,32);

	mbag_set_byte(wlan,"key_index",cw_get_byte(data+41));
	mbag_set_byte(wlan,"key_shared",cw_get_byte(data+42));
	
	mbag_set_byte(wlan,"wpa_len",cw_get_byte(data+43));
	mbag_set_bstr16n(wlan,"wpa_data",data+44,32);

	mbag_set_byte(wlan,"rsn_len",cw_get_byte(data+76));
	mbag_set_bstr16n(wlan,"rsn_data",data+77,64);

	mbag_set_bstr16n(wlan,"reserved",data+141,49);

	mbag_set_byte(wlan,"wme_len",cw_get_byte(data+190));	
	mbag_set_bstr16n(wlan,"wme_data",data+191,32);	

	mbag_set_byte(wlan,"dot11e_len",cw_get_byte(data+223));	
	mbag_set_bstr16n(wlan,"dot11e_data",data+224,32);	

	mbag_set_byte(wlan,"qos",cw_get_byte(data+256));	

//	mbag_set_byte(wlan,"ssid_broadcast",cw_get_byte(data+257));
	mbag_set_byte(wlan,"ssid_broadcast",cw_get_byte(data+435));
	mbag_set_byte(wlan,"aironet_ie",cw_get_byte(data+436));

	mbag_set_bstr16n(wlan,"reserved2",data+258,40);


	mbag_set_byte(wlan,"dtim_period",cw_get_byte(data+554));
	mbag_set_bstr16n(wlan,"wlan_name",data+558,32);
	mbag_set_byte(wlan,"allow_aaa_override",cw_get_byte(data+591));
	mbag_set_byte(wlan,"max_clients",cw_get_byte(data+593));

	
	mbag_set_bstr16n(wlan,"ssid",data+622,len-622);

	return 0;
}


int cw_cisco_get_wlan_(mbag_t wlan, uint8_t *data, int len)
{
	mbag_set_word(wlan,"enc_capab", cw_get_word(data+1));

	int wlan_id=cw_get_word(data+3);
	mbag_set_word(wlan,"wlan_id",wlan_id);

	mbag_set_dword(wlan,"enc_policy",cw_get_dword(data+5));
	mbag_set_bstr16n(wlan,"key",data+9,32);

	mbag_set_byte(wlan,"key_index",cw_get_byte(data+41));
	mbag_set_byte(wlan,"key_shared",cw_get_byte(data+42));
	
	mbag_set_byte(wlan,"wpa_len",cw_get_byte(data+43));
	mbag_set_bstr16n(wlan,"wpa_data",data+44,32);

	mbag_set_byte(wlan,"rsn_len",cw_get_byte(data+76));
	mbag_set_bstr16n(wlan,"rsn_data",data+77,64);

	mbag_set_bstr16n(wlan,"reserved",data+141,49);

	mbag_set_byte(wlan,"wme_len",cw_get_byte(data+190));	
	mbag_set_bstr16n(wlan,"wme_data",data+191,32);	

	mbag_set_byte(wlan,"dot11e_len",cw_get_byte(data+223));	
	mbag_set_bstr16n(wlan,"dot11e_data",data+224,32);	

	mbag_set_byte(wlan,"qos",cw_get_byte(data+256));	

//	mbag_set_byte(wlan,"ssid_broadcast",cw_get_byte(data+257));
	mbag_set_byte(wlan,"ssid_broadcast",cw_get_byte(data+435));
	mbag_set_byte(wlan,"aironet_ie",cw_get_byte(data+436));

	mbag_set_bstr16n(wlan,"reserved2",data+258,40);


	mbag_set_byte(wlan,"dtim_period",cw_get_byte(data+554));
	mbag_set_bstr16n(wlan,"wlan_name",data+558,32);
	mbag_set_byte(wlan,"allow_aaa_override",cw_get_byte(data+591));
	mbag_set_byte(wlan,"max_clients",cw_get_byte(data+593));

	
	mbag_set_bstr16n(wlan,"ssid",data+622,len-622);

	return 0;
}






int cisco_in_add_wlan(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	int rid = cw_get_byte(data);
	mbag_t wlan = mbag_i_get_mbag(conn->radios,rid,NULL);
	if ( !wlan ) {
		printf("No Radio git\n");


	}

	mbag_set_byte(wlan,"radio_id",rid);


	int mytype=mbag_get_byte(conn->config,CISCO_ITEM_AP_MODE_AND_TYPE,77) & 0xff;

	printf("My type: %d\n",mytype);
	exit(0);

	

	cw_cisco_get_wlan_legacy(wlan,data,len);




	return 0;


}


