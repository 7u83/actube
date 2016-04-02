#include "cw.h"
#include "capwap80211_items.h"
#include "dbg.h"

int cw_read_80211_mac_operation(uint8_t *data,mbag_t r)
{
	mbag_set_word( r, CW_RADIOITEM80211_RTS_THRESHOLD,cw_get_word(data) );
	mbag_set_byte( r, CW_RADIOITEM80211_SHORT_RETRY,cw_get_byte(data+2) );
	mbag_set_byte( r, CW_RADIOITEM80211_LONG_RETRY,cw_get_byte(data+3) );
	mbag_set_word( r, CW_RADIOITEM80211_FRAGMENTATION_THRESHOLD,cw_get_word(data+4) );
	mbag_set_dword( r, CW_RADIOITEM80211_TX_MSDU_LIFETIME,cw_get_dword(data+6) );
	mbag_set_dword( r, CW_RADIOITEM80211_RX_MSDU_LIFETIME,cw_get_dword(data+10) );
	return 1;
}
