#include "wtpdrv.h"


wpa_printf()
{
}

wpa_hexdump()
{
}




#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/ctrl.h"
#include <netlink/msg.h>
#include <net/if.h>

#include "nl80211.h"


static int family_id;

static int nlCallback(struct nl_msg *msg, void *arg)
{

	printf("Enter callback\n");


	struct nlmsghdr *ret_hdr = nlmsg_hdr(msg);
	struct nlattr * tb_msg[NL80211_ATTR_MAX + 1];

//	printf("AMAX = %d\n",NL80211_ATTR_MAX);
//	printf("WIPI = %d\n",NL80211_ATTR_WIPHY_NAME);

	int ii;
	for (ii=0; ii<NL80211_ATTR_MAX; ii++){
		tb_msg[ii]=0;
	}
	if (!tb_msg[NL80211_ATTR_WIPHY_NAME]) {
//		printf("still no mame\n");
	}
	else{
//		printf("a name her????\n");
	}



	if (ret_hdr->nlmsg_type != family_id) {
		printf("Wrong family\n");
		// what is this??
		return NL_STOP;
	}

	struct genlmsghdr *gnlh =
	    (struct genlmsghdr *) nlmsg_data(ret_hdr);

	int cmd = gnlh->cmd;

	printf ("CMD: %d\n",cmd);

	struct nlattr * head = genlmsg_attrdata(gnlh, 0);
	int alen = genlmsg_attrlen(gnlh, 0);
	int rem;

	struct nlattr *nla;
	nla_for_each_attr(nla,head,alen,rem){

		printf("ATR Type: %d\n",nla->nla_type);


	}

	
/*
	nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), NULL);


	if (tb_msg[NL80211_ATTR_WIPHY_NAME]) {
//		int type = nla_get_u32(tb_msg[NL80211_ATTR_IFTYPE]);

//		printf("Type: %d\n", type);
		struct nlattr * a = tb_msg[NL80211_ATTR_WIPHY_NAME];

		printf ("ATTR %p\n",a);


		char * str = nla_get_string(a);

		printf("Has namei %s\n",str);
	}
*/
	return NL_OK;

}


void gr()
{

	int ret;
	struct nl_sock *sk = nl_socket_alloc();

	genl_connect(sk);
	family_id = genl_ctrl_resolve(sk, "nl80211");

	printf("ExpId %d\n", family_id);

	//attach a callback
	nl_socket_modify_cb(sk, NL_CB_VALID, NL_CB_CUSTOM, nlCallback,
			    NULL);

	//allocate a message
	struct nl_msg *msg = nlmsg_alloc();


	printf("Command assigned\n");

	int ifIndex = if_nametoindex("wlan1");
	int flags = NLM_F_DUMP;

	printf("If idx %d\n", ifIndex);

	/* setup the message */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, flags,
		    NL80211_CMD_GET_WIPHY, 0);


	printf("gennlmsg\n");

	//add message attributes
//	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifIndex);

//	NLA_PUT_FLAG(msg, NL80211_ATTR_SPLIT_WIPHY_DUMP);

	printf("NLAPIT\n");
	//send the messge (this frees it)
	ret = nl_send_auto_complete(sk, msg);
	printf("Send ret %d\n",ret);

	int nlr;

	//block for message to return
	nlr = nl_recvmsgs_default(sk);
	printf("NLR = %d\n",nlr);



	return 0;

      nla_put_failure:
	nlmsg_free(msg);
	return 1;

}
















int wtpdrv_get_num_radios()
{
	return 4;
}


/*
static int wtpdrv_get_radioinfo(int rid,struct radioinfo * radioinfo)
{
	struct wpa_driver_ops * drv = wpa_drivers[0];

	struct hostapd_hw_modes * hwm;
	uint16_t nummodes;
	uint16_t fl;

	radioinfo->rid=rid;

	hwm = drv->get_hw_feature_data(drv_priv[rid-1],&nummodes,&fl);

	int i;
	for (i=0; i<nummodes; i++){
		printf ("Mode %d\n",i);
		printf ("------------------------\n");
		printf ("Num channels: %d\n",hwm[i].num_channels);



		printf ("Hwmod: %d\n",hwm[i].mode);

		int mode = hwm[i].mode;
		switch(mode){
			case HOSTAPD_MODE_IEEE80211B:
				radioinfo->type|=CWRADIO_TYPE_B;
				printf("Mode B\n");
				break;
			case HOSTAPD_MODE_IEEE80211G:
				radioinfo->type|=CWRADIO_TYPE_G;
				printf("Mode G\n");
				break;
			case HOSTAPD_MODE_IEEE80211A:
				radioinfo->type|=CWRADIO_TYPE_A;
				printf("Mode A\n");
				break;
			case NUM_HOSTAPD_MODES:
				printf("NIMA AP MODES\n");
				break;

		}
		if (hwm[i].ht_capab)
			radioinfo->type|=CWRADIO_TYPE_N;

		printf ("ht_capab: %d\n",hwm[i].ht_capab);
		printf ("flags: %d\n",hwm[i].flags);
		printf("\n");
	}



//////////////	printf("hem %p\n",hwm);

//	drv
	


}

*/
