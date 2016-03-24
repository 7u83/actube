#include <netlink/genl/genl.h>


#include "nlt.h"


int nlt_parse(struct nl_msg *msg, struct nlt_msg *r)
{
	struct nlmsghdr *msghdr = nlmsg_hdr(msg);
	struct genlmsghdr *ghdr = (struct genlmsghdr *) nlmsg_data(msghdr);
	int rc;
	rc = nla_parse(r->attribs, NL80211_ATTR_MAX, genlmsg_attrdata(ghdr, 0),
		       genlmsg_attrlen(ghdr, 0), NULL);
	if (rc < 0)
		return rc;
	r->cmd = ghdr->cmd;
	return 0;
}


