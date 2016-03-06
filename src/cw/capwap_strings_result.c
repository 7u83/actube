#include "capwap.h"


struct cw_strlist_elem capwap_strings_result[] = {

	{CW_RESULT_SUCCESS,"Success"},							/* 0 */
	{CW_RESULT_MISSING_AC_LIST,"AC List Message Element MUST be Present"},		/* 1 */
	{CW_RESULT_SUCCESS_NAT,"Success - NAT Detected"},				/* 2 */
	{CW_RESULT_RESET_UNABLE_TO_RESET,"Reset Failure - Unable to Reset"},		/* 10 */
	{CW_RESULT_IMAGE_DATA_INVALID_CHECKSUM,"Image Data Error (Invalid Checksum)"},			/* 14 */
	{CW_RESULT_IMAGE_DATA_ERROR,"Image Data Error (Unspecified)"},					/* 16 */
	{CW_RESULT_MSG_INVALID_IN_CURRENT_STATE,"Message unexpected (Invalid in current state)"},	/* 18 */
	{CW_RESULT_MISSING_MAND_ELEM,"Missing Mandatory Message Element"},		/* 20 */
	{CW_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT,"Unrecognized Message Element"},	/* 21 */
	{CW_STR_STOP,"Unknown Result Code"}

};
	
