#ifndef __NLT_H
#define __NLT_H

#include "nl80211.h"
#include <netlink/netlink.h>
#include <netlink/socket.h>
#include "netlink/msg.h"

extern const char * nlt_attrnames[];
extern const char * nlt_cmdnames[];

#define nlt_get_attrname(type) (nlt_attrnames[type])
#define nlt_get_cmdname(cmd) (cmd>NL80211_CMD_MAX? "Unknown": nlt_cmdnames[cmd])

struct nlt_ifinfo {
	const char *ifname;
	int ifindex;
	int iftype;
};


struct nlt_wiphyinfo {
	int index;
	const char * name;
};

struct nlt_msg {
	int cmd;
	struct nlattr *attribs[NL80211_ATTR_MAX + 1];
};



int nlt_get_ifinfo(struct nl_sock *sk, struct nlt_ifinfo *ifinfo);
int nlt_init_ifinfo(struct nlt_ifinfo * ifinfo);
//int nlt_get_wiphy_list(struct nlt_ifinfo * ifinfo);
int nlt_get_wiphy_list(struct nl_sock *sk);

extern int nlt_parse(struct nl_msg *msg, struct nlt_msg *r);



#define NLT_MAX_WIPHYINDEX	8

#endif

