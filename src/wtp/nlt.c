#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/ctrl.h"
#include <netlink/msg.h>


#include "nlt.h"

/*
int nlt_send_and_recvmsg(struct nl_sock *sk, struct nl_msg *msg)
{
	struct nl_cb * cb = nl_socket_get_cb(sk);

}

*/

/*
struct nlt_ifinfo {
	const char *ifname;
	int ifindex;
	int iftype;
};

static int get_ifinfo_cb(struct nl_msg *msg, void *arg)
{
	struct nlt_ifinfo *cb_data = (struct ifindex_cb_data *) arg;

	printf("Yeaaaaaaaaaa! The ifindex callback is here\n");
	struct nlmsghdr *msghdr = nlmsg_hdr(msg);
	struct nlattr *msgattribs[NL80211_ATTR_MAX + 1];

	struct genlmsghdr *ghdr = (struct genlmsghdr *) nlmsg_data(msghdr);

	int rc;
	rc = nla_parse(msgattribs, NL80211_ATTR_MAX,
		       genlmsg_attrdata(ghdr, 0), genlmsg_attrlen(ghdr, 0),
		       NULL);

	if (rc < 0) {
		cw_dbg(DBG_DRV_ERR, "nla_parse failed: %d %d", rc,
		       nl_geterror(rc));
		return NL_SKIP;
	}

	int cmd = ghdr->cmd;
	cw_dbg(DBG_DRV, "NL Callback, cmd=%d - %s", cmd,
	       nlt_get_cmdname(cmd));

	if (cmd != NL80211_CMD_NEWINTERFACE)
		return NL_OK;


	if (cmd == NL80211_CMD_NEW_INTERFACE) {
		if (msgattribs[NL80211_ATTR_IFNAME]) {
			printf("IFNAME = %s\n",
			       nla_get_string(msgattribs
					      [NL80211_ATTR_IFNAME]));

			cb_data->index =
			    nla_get_u32(msgattribs[NL80211_ATTR_IFINDEX]);
			cb_data->type =
			    nla_get_u32(msgattribs[NL80211_ATTR_IFTYPE]);


		}

	}

}



*/

int nlt_get_ifinfo(const char *ifname,struct nlt_ifinfo * ifinfo)
{

	return 0;
}


