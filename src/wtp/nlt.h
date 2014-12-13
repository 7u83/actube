#ifndef __NLT_H
#define __NLT_H

#include "nl80211.h"

extern const char * nlt_attrnames[];
extern const char * nlt_cmdnames[];

#define nlt_get_attrname(type) nlt_attrnames[type]
#define nlt_get_cmdname(cmd) (cmd>NL80211_CMD_MAX? "Unknown": nlt_cmdnames[cmd])

struct nlt_ifinfo {
	const char *ifname;
	int ifindex;
	int iftype;
};


#endif

