#ifndef __CAPWAP80211_ITEMS_H
#define __CAPWAP80211_ITEMS_H

#include "item.h"

extern const char CW_RADIOITEM80211_SUPPORTED_RATES[];
extern const char CW_RADIOITEM80211_RATE_SET[];
extern const char CW_RADIOITEM80211_WTP_RADIO_INFORMATION[];

extern struct cw_itemdef capwap80211_itemdefs[];
extern struct cw_itemdef capwap80211_radioitemdefs[];


/* MAC Operation Items */
extern const char CW_RADIOITEM80211_RTS_THRESHOLD[];
extern const char CW_RADIOITEM80211_FRAGMENTATION_THRESHOLD[];
extern const char CW_RADIOITEM80211_SHORT_RETRY[];
extern const char CW_RADIOITEM80211_LONG_RETRY[];
extern const char CW_RADIOITEM80211_TX_MSDU_LIFETIME[];
extern const char CW_RADIOITEM80211_RX_MSDU_LIFETIME[];


#endif

