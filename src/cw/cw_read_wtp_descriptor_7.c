#include "cw.h"

#include "capwap.h"

#include "ktv.h"
#include "keys.h"



static struct cw_DescriptorSubelemDef allowed_default[] = {
	{-1,CW_SUBELEM_WTP_HARDWARE_VERSION, CW_SKEY_HARDWARE, 1024,0},
	{-1,CW_SUBELEM_WTP_SOFTWARE_VERSION, CW_SKEY_SOFTWARE, 1024.0},
	{-1,CW_SUBELEM_WTP_BOOTLOADER_VERSION, CW_SKEY_BOOTLOADER, 1024,0},
	{-1,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, CW_SKEY_OTHER_SOFTWARE, 1024,0},
	{0,0, NULL, 0,0}
};



/**
 * Read WTP Descriptor in Cisco-Style (Draft 7)
 */
int cw_read_wtp_descriptor_7(mavl_t cfg, struct conn *conn,
			     struct cw_ElemHandler *eh, uint8_t * data, int len,
			     struct cw_DescriptorSubelemDef *allowed)
{

/*	int ncrypt; //currentliy unused */
	int pos;
	char key[64];

	sprintf(key,"%s/%s",eh->key, CW_SKEY_MAX_RADIOS);
	cw_ktv_add(cfg,key,CW_TYPE_BYTE,data,1);

	sprintf(key,"%s/%s",eh->key, CW_SKEY_RADIOS_IN_USE);
	cw_ktv_add(cfg,key,CW_TYPE_BYTE,data+1,1);
	

	pos = 2;

	/* Encryption element, for now dumy XXX */
/*	//cw_get_word(data + pos + 2);*/
	pos += 2;


	if (!allowed)
		allowed=allowed_default;

	return cw_read_descriptor_subelems(cfg, eh->key, data + pos, len - pos, allowed);
}
