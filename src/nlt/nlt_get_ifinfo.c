#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>

#include "nlt.h"

static int error_handler(struct sockaddr_nl *nla, struct nlmsgerr *err, void *arg)
{

	printf("Error handler ------lllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
	int *ret = arg;
	*ret = err->error;
	printf("Err: %d\n", *ret);
	return NL_SKIP;
}



static int get_ifinfo_cb(struct nl_msg *msg, void *arg)
{
	struct nlt_ifinfo *ifinfo = (struct nlt_ifinfo *) arg;

	struct nlt_msg r;
	nlt_parse(msg, &r);


	if (r.cmd != NL80211_CMD_NEW_INTERFACE)
		return NL_OK;


	if (ifinfo->ifname != NULL) {
		printf("! if info != 0\n");

		if (!r.attribs[NL80211_ATTR_IFNAME])
			return NL_OK;

		const char *cbname = nla_get_string(r.attribs[NL80211_ATTR_IFNAME]);
		printf("Callback name %s\n", cbname);

		if (strcmp(ifinfo->ifname, nla_get_string(r.attribs[NL80211_ATTR_IFNAME]))
		    != 0)
			return NL_OK;
	}


	if (r.attribs[NL80211_ATTR_IFNAME]) {
		if (ifinfo->ifname == NULL)
			ifinfo->ifname = strdup(nla_get_string(r.attribs[NL80211_ATTR_IFNAME]));
	}

	if (r.attribs[NL80211_ATTR_IFINDEX])
		ifinfo->ifindex = nla_get_u32(r.attribs[NL80211_ATTR_IFINDEX]);

	if (r.attribs[NL80211_ATTR_IFTYPE])
		ifinfo->iftype = nla_get_u32(r.attribs[NL80211_ATTR_IFTYPE]);



	return NL_OK;
}





int nlt_get_ifinfo(struct nl_sock *sk, struct nlt_ifinfo *ifinfo)
{

	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return -1;

	int flags = NLM_F_DUMP;

	int family_id = genl_ctrl_resolve(sk, "nl80211");

	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, flags, NL80211_CMD_GET_INTERFACE, 0);

//      NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, 0);
	nl_send_auto(sk, msg);




	int err;

	struct nl_cb *nl_cb = nl_cb_alloc(NL_CB_CUSTOM);
	nl_cb_set(nl_cb, NL_CB_VALID, NL_CB_CUSTOM, get_ifinfo_cb, ifinfo);
	nl_cb_err(nl_cb, NL_CB_CUSTOM, error_handler, &err);


	int nlr;
//      do {
	nlr = nl_recvmsgs(sk, nl_cb);
//              printf("round %d\n",nlr);


//      }while(1);;

	//int nlr = nl_recvmsgs_default(sk);
//	cw_log(LOG_ERR, "iGet if index: Make if %d - %s", nlr, nl_geterror(nlr));




//      nla_put_failure:
	nlmsg_free(msg);
	return 0;


}


