#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/ctrl.h"
#include <netlink/msg.h>

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/avltree.h"

#include "nlt.h"


/*

static int error_handler(struct sockaddr_nl *nla, struct nlmsgerr *err, void *arg)
{

	printf("Error handler ------lllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
	int *ret = arg;
	*ret = err->error;
	printf("Err: %d\n", *ret);
	return NL_SKIP;
}


static int wiphylist_cmp(const void * d1,const void *d2)
{
	struct nlt_wiphyinfo * c1=(struct nlt_wiphy *) d1;
	struct nlt_wiphyinfo * c2=(struct nlt_wiphy *) d2;

	return c1->index - c2->index;
}


struct avltree * wiphylist_create()
{
	return avltree_create(wiphylist_cmp,0);
}


struct nlt_wiphyinfo * wiphylist_get( struct avltree * l,int idx)
{
	//return avltree_get(l);
}


struct nlt_wiphyinfo * nlt_wiphylist_add(struct avltree * t, struct nlt_wiphyinfo * wi)
{
	return avltree_add(t,wi);
}




static int add_wiphyinfo(struct nlattr *msgattribs[NL80211_ATTR_MAX + 1],
			 struct nlt_wiphyinfo **wiphyinfos)
{
	if (!msgattribs[NL80211_ATTR_WIPHY]) {
		return NL_SKIP;
	}

	int index = nla_get_u32(msgattribs[NL80211_ATTR_WIPHY]);


	struct nlt_wiphyinfo *wi = wiphyinfos[index];

	if (!wi) {
		wi = malloc(sizeof(struct nlt_wiphyinfo));
		memset(wi, 0, sizeof(struct nlt_wiphyinfo));
		wiphyinfos[index] = wi;
	}


	if (msgattribs[NL80211_ATTR_WIPHY_NAME] && !wi->name) {
		wi->name = strdup(nla_get_string(msgattribs[NL80211_ATTR_WIPHY_NAME]));
	}



	return 0;

}

















struct nlt_msg {
	int cmd;
	struct nlattr *attribs[NL80211_ATTR_MAX + 1];
};

static int nlt_parse(struct nl_msg *msg, struct nlt_msg *r)
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


int nlt_init_ifinfo(struct nlt_ifinfo *ifinfo)
{
	memset(ifinfo, 0, sizeof(struct nlt_ifinfo));
	ifinfo->ifindex = -1;
}


struct nl_cb *get_nl_cb(int (*fun) (struct nl_msg *, void *arg), void *arg)
{
	struct nl_cb *nl_cb = nl_cb_alloc(NL_CB_CUSTOM);
	if (!nl_cb)
		return 0;

	int err;
	nl_cb_set(nl_cb, NL_CB_VALID, NL_CB_CUSTOM, fun, arg);
	nl_cb_err(nl_cb, NL_CB_CUSTOM, error_handler, &err);
	return nl_cb;
}


struct nl_msg *nlt_nl_msg_new(struct nl_sock * sk, int cmd, int flags)
{
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return 0;


	int family_id = genl_ctrl_resolve(sk, "nl80211");

	genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id, 0, flags, cmd, 0);
	return msg;

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
	cw_log(LOG_ERR, "iGet if index: Make if %d - %s", nlr, nl_geterror(nlr));




//      nla_put_failure:
	nlmsg_free(msg);
	return 0;


}



static int nlCallback(struct nl_msg *msg, void *arg)
{

	struct nlmsghdr *msghdr = nlmsg_hdr(msg);
	struct nlattr *msgattribs[NL80211_ATTR_MAX + 1];

	struct genlmsghdr *ghdr = (struct genlmsghdr *) nlmsg_data(msghdr);

	int rc;
	rc = nla_parse(msgattribs, NL80211_ATTR_MAX, genlmsg_attrdata(ghdr, 0),
		       genlmsg_attrlen(ghdr, 0), NULL);

	if (rc < 0) {
		cw_dbg(DBG_DRV_ERR, "nla_parse failed: %d %d", rc, nl_geterror(rc));
		return NL_SKIP;
	}

	int cmd = ghdr->cmd;
//      cw_dbg(DBG_DRV,"NL Callback, cmd=%d - %s",cmd,nlt_get_cmdname(cmd));

	switch (cmd) {
		case NL80211_CMD_NEW_WIPHY:
			add_wiphy_data(msgattribs, arg);
			break;
//              case NL80211_CMD_NEW_INTERFACE:
//                      add_interface_data(msgattribs);

		default:
			printf("Default\n");



	}

	return NL_OK;

}














static int get_wiphy_info_cb(struct nl_msg * msg,void * arg)
{
	struct nlt_msg m;
	nlt_parse(msg, &m);

	if (m.cmd != NL80211_CMD_NEW_WIPHY)
		return NL_SKIP;

	if (!m.attribs[NL80211_ATTR_WIPHY])
		return NL_SKIP;

	
	int index =  nla_get_u32(m.attribs[NL80211_ATTR_WIPHY]);
	if (index > NLT_MAX_WIPHYINDEX)
		return NL_SKIP;

	struct nlt_wiphyinfo ** wi = (struct nlt_wiphyinfo**)arg;

	if (wi[index]==0){
		wi[index]=malloc( sizeof(struct nlt_wiphyinfo));
		if (wi[index]==0)
			return NL_SKIP;
		memset(wi[index],0,sizeof(struct nlt_wiphyinfo));
	}



	if (m.attribs[NL80211_ATTR_WIPHY_NAME]){
		if (wi[index]->name)	
			free(wi[index]->name);
		char * name = nla_get_string(m.attribs[NL80211_ATTR_WIPHY_NAME]);
		wi[index]->name=strdup(name);
	}
		

}



int nlt_get_wiphy_list(struct nl_sock *sk)
{

	struct nlt_wiphyinfo ** wi = malloc (sizeof(struct nlt_wiphyinfo *)*NLT_MAX_WIPHYINDEX);
	if (wi==0)
		return 0;
	memset (wi,0,sizeof(struct nlt_wiphyinfo *)*NLT_MAX_WIPHYINDEX);


	struct nl_msg * msg = nlt_nl_msg_new(sk,NL80211_CMD_GET_WIPHY,NLM_F_DUMP);
	nl_send_auto(sk, msg);
	struct nl_cb *nl_cb = get_nl_cb(get_wiphy_info_cb,wi);
//	while(1){
		int nlr = nl_recvmsgs(sk, nl_cb);
//	}


	int i;
	for(i=0; i< NLT_MAX_WIPHYINDEX; i++){
		if ( wi[i] ){
			printf("Found wiphy on %d with name %s\n",i,wi[i]->name);
		}


	}

}


*/
