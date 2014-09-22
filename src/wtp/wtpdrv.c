#include <errno.h>



#include "wtpdrv.h"
#include "nlt.h"


#include "capwap/cw_log.h"
#include "capwap/radioinfo.h"

wpa_printf()
{
}

wpa_hexdump()
{
}


#include <linux/types.h>

#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/ctrl.h"
#include <netlink/msg.h>
#include <net/if.h>

#include "nl80211.h"

/*
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int bool;
typedef signed char s8;
#define false 0
#define true (!0)
#define __packed __attribute__ ((packed))

typedef __le16 le16
*/


#include <sys/ioctl.h>

#include "dot11.h"
//#include "ieee802_11_defs.h"


struct radioinfo radioinfos[31];
static struct nl_sock * sk;


int interface_up(const char * ifname)
{
	int sockfd;
	struct ifreq ifr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0)
		return 0;

	memset(&ifr, 0, sizeof ifr);

	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	ifr.ifr_flags |= IFF_UP;
	ioctl(sockfd, SIOCSIFFLAGS, &ifr);
}


struct hostapd_freq_params {
        int mode;
        int freq;
        int channel;
        /* for HT */
        int ht_enabled;
        int sec_channel_offset; /* 0 = HT40 disabled, -1 = HT40 enabled,
                                 * secondary channel below primary, 1 = HT40
                                 * enabled, secondary channel above primary */

        /* for VHT */
        int vht_enabled;

        /* valid for both HT and VHT, center_freq2 is non-zero
         * only for bandwidth 80 and an 80+80 channel */
        int center_freq1, center_freq2;
        int bandwidth;
};




static int nlt_put_freq_params(struct nl_msg *msg,
                                   struct hostapd_freq_params *freq)
{
        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, freq->freq);
        if (freq->vht_enabled) {
                switch (freq->bandwidth) {
                case 20:
                        NLA_PUT_U32(msg, NL80211_ATTR_CHANNEL_WIDTH,
                                    NL80211_CHAN_WIDTH_20);
                        break;
                case 40:
                        NLA_PUT_U32(msg, NL80211_ATTR_CHANNEL_WIDTH,
                                    NL80211_CHAN_WIDTH_40);
                        break;
                case 80:
                        if (freq->center_freq2)
                                NLA_PUT_U32(msg, NL80211_ATTR_CHANNEL_WIDTH,
                                            NL80211_CHAN_WIDTH_80P80);
                        else
                                NLA_PUT_U32(msg, NL80211_ATTR_CHANNEL_WIDTH,
                                            NL80211_CHAN_WIDTH_80);
                        break;
                case 160:
                        NLA_PUT_U32(msg, NL80211_ATTR_CHANNEL_WIDTH,
                                    NL80211_CHAN_WIDTH_160);
                        break;
                default:
                        return -EINVAL;
                }
                NLA_PUT_U32(msg, NL80211_ATTR_CENTER_FREQ1, freq->center_freq1);
                if (freq->center_freq2)
                        NLA_PUT_U32(msg, NL80211_ATTR_CENTER_FREQ2,
                                    freq->center_freq2);
        } else if (freq->ht_enabled) {
                switch (freq->sec_channel_offset) {
                case -1:
                        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
                                    NL80211_CHAN_HT40MINUS);
                        break;
                case 1:
                        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
                                    NL80211_CHAN_HT40PLUS);
                        break;
                default:
                        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
                                    NL80211_CHAN_HT20);
                        break;
                }
        }
        return 0;

nla_put_failure:
        return -ENOBUFS;
}


/*
static int set_channel( struct hostapd_freq_params *freq, int set_chan)
{
        struct nl_msg *msg;

        msg = nlmsg_alloc();
        if (!msg)
                return -1;

        nl80211_cmd(drv, msg, 0, set_chan ? NL80211_CMD_SET_CHANNEL :
                    NL80211_CMD_SET_WIPHY);

	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, 0,
		    NL80211_CMD_SET_WIPHY, 0);


        NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, drv->ifindex);
        if (nl80211_put_freq_params(msg, freq) < 0)
                goto nla_put_failure;

        ret = send_and_recv_msgs(drv, msg, NULL, NULL);
        msg = NULL;
        if (ret == 0) {
                bss->freq = freq->freq;
                return 0;
        }
nla_put_failure:
        nlmsg_free(msg);
        return -1;
}
*/










static int family_id;



static int add_wiphy_data(struct nlattr* msgattribs[NL80211_ATTR_MAX+1])
{
	const char * name;
	if (!msgattribs[NL80211_ATTR_WIPHY]){
		printf("Error, on index\n");
		return 0;
	}
	int index = nla_get_u32( msgattribs[NL80211_ATTR_WIPHY] );
	printf("Index is %d\n",index);


	if (msgattribs[NL80211_ATTR_WIPHY_BANDS]){
		printf("Yea band\n");	
		struct nlattr *nla=msgattribs[NL80211_ATTR_WIPHY_BANDS];
		struct nla_attr * cnla;
		int rem_nla;

		/* TODO parse bands */
/*		nla_for_each_nested(cnla, nla, rem_nla) {
			int band = nla_get_u16(cnla);
			printf("Band found: %d\n",band);
		}

*/


	

	}


	int i;
	for (i=0; i<NL80211_ATTR_MAX; i++){
		if (msgattribs[i]){
			printf("Attrib %d, %s\n",i,nlt_get_attrname(i));
		}
	}

}


static int nlCallback(struct nl_msg *msg, void *arg)
{

	struct nlmsghdr *msghdr = nlmsg_hdr(msg);
	struct nlattr *msgattribs[NL80211_ATTR_MAX + 1];

	struct genlmsghdr *ghdr =
	    (struct genlmsghdr *) nlmsg_data(msghdr);

	int rc;
	rc = nla_parse(msgattribs, NL80211_ATTR_MAX, genlmsg_attrdata(ghdr, 0),
		  genlmsg_attrlen(ghdr, 0), NULL);

	if (rc<0){
		cw_dbg(DBG_DRV_ERR,"nla_parse failed: %d %d",rc,nl_geterror(rc));
		return NL_SKIP;
	}

	int cmd = ghdr->cmd;
	cw_dbg(DBG_DRV,"NL Callback, cmd=%d - %s",cmd,nlt_get_cmdname(cmd));

	switch(cmd){
		case NL80211_CMD_NEW_WIPHY:
			add_wiphy_data(msgattribs);
			break;
		default:
			printf("Default\n");



	}

	return NL_OK;
	
}














static int xnlCallback(struct nl_msg *msg, void *arg)
{

	printf("NLCallback\n");

	struct nlmsghdr *ret_hdr = nlmsg_hdr(msg);
	struct nlattr *tb_msg[NL80211_ATTR_MAX + 1];

//      printf("AMAX = %d\n",NL80211_ATTR_MAX);
//      printf("WIPI = %d\n",NL80211_ATTR_WIPHY_NAME);

	int ii;
	for (ii = 0; ii < NL80211_ATTR_MAX; ii++) {
		tb_msg[ii] = 0;
	}
	if (!tb_msg[NL80211_ATTR_WIPHY_NAME]) {
//              printf("still no mame\n");
	} else {
//              printf("a name her????\n");
	}



	if (ret_hdr->nlmsg_type != family_id) {
		printf("Wrong family\n");
		// what is this??
		return NL_STOP;
	}

	struct genlmsghdr *gnlh =
	    (struct genlmsghdr *) nlmsg_data(ret_hdr);

	int cmd = gnlh->cmd;

	printf("CMD: %d - %s\n", cmd, nlt_get_cmdname(cmd));

	struct nlattr *head = genlmsg_attrdata(gnlh, 0);
	int alen = genlmsg_attrlen(gnlh, 0);
	int rem;

	struct nlattr *nla;
	nla_for_each_attr(nla, head, alen, rem) {

		switch (nla->nla_type) {
			case NL80211_ATTR_IFINDEX:
				{
					int idx = nla_get_u32(nla);
					printf("IF IFXC = %d\n", idx);
					break;
				}
			case NL80211_ATTR_IFNAME:
				{
					const char * nam  = nla_get_string(nla);
					printf("IF iNAM = %s\n", nam);
					break;
				}
		}





		//printf("ATR Type: %d - %s\n",nla->nla_type,nlt_get_attrname(nla->nla_type));

		if (nla->nla_type == NL80211_ATTR_SUPPORTED_COMMANDS) {

			printf("Supported commands found, len = %d\n",
			       nla->nla_len);
			struct nlattr *nl_cmd;
			int rem_cmd;
			nla_for_each_nested(nl_cmd, nla, rem_cmd) {
				int cmd = nla_get_u32(nl_cmd);
				const char *n = nlt_get_cmdname(cmd);
				printf("Supported command: %d, %s\n", cmd,
				       n);
			}




		}



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


void make_if()
{
	const char * ifname = "moni";
	
	/* allocate a message */
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return;

	/* init message */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, 0,
		    NL80211_CMD_NEW_INTERFACE, 0);


	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, NL80211_IFTYPE_AP);
	NLA_PUT_STRING(msg, NL80211_ATTR_IFNAME, ifname);

	int rc = nl_send_auto_complete(sk, msg);


	int nlr = nl_recvmsgs_default(sk);
	printf("NLR = %d\n", nlr);

	interface_up(ifname);


      nla_put_failure:
	nlmsg_free(msg);
	return 1;
}


void start_ap(struct nl_sock *sk)
{
	/* allocate a message */
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return;



	/* init message */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST,
		    NL80211_CMD_START_AP, 0);


	struct dot11_mgmt *head = NULL;




	head = malloc(256);
	uint8_t * tail;
	tail = malloc(512);

	head->frame_control = htons (DOT11_FTYPE_MGMT | DOT11_STYPE_BEACON) ; 

//			 = IEEE80211_FC(WLAN_FC_TYPE_MGMT,
//					   WLAN_FC_STYPE_BEACON);
	head->duration = htons(0);
	/* destination address */
	memset(head->da, 0xff, sizeof(head->da));
	
	



	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, 8);
	NLA_PUT_U16(msg, NL80211_ATTR_BEACON_INTERVAL, 100);
	NLA_PUT_U32(msg, NL80211_ATTR_DTIM_PERIOD, 1);
	NLA_PUT_U32(msg, NL80211_ATTR_CIPHER_SUITES_PAIRWISE, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_CIPHER_SUITE_GROUP, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_AKM_SUITES, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_AUTH_TYPE, NL80211_AUTHTYPE_OPEN_SYSTEM);


	NLA_PUT_U32(msg, NL80211_ATTR_WPA_VERSIONS, NL80211_WPA_VERSION_2);


	const char *ssid = "HelloWorld";
	NLA_PUT_U32(msg, NL80211_ATTR_HIDDEN_SSID, NL80211_HIDDEN_SSID_NOT_IN_USE);
	NLA_PUT(msg, NL80211_ATTR_SSID, strlen(ssid), ssid);
//      nla_put(msg,NL80211_ATTR_SSID,ssid,strlen(ssid));
	printf("Sot ssid\n");


	int ret = nl_send_auto_complete(sk, msg);
	printf("AP IF Send ret %d\n", ret);

	int nlr = nl_recvmsgs_default(sk);
	printf("Start AP NLR = %d\n", nlr);


	msg = nlmsg_alloc();

	/* init message */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST,
		    NL80211_CMD_SET_CHANNEL, 0);

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, 8);
        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, 2412);
	int rc = nl_send_auto_complete(sk, msg);

	printf ("Set Channel RC: %d\n",rc);

	nlr = nl_recvmsgs_default(sk);
	printf("Start Channel AP NLR = %d\n", nlr);








      nla_put_failure:
	printf("Bau\n");
	nlmsg_free(msg);
	return 1;


}


int init()
{
	cw_log(LOG_INFO,"Starting nlsock");
	sk = nl_socket_alloc();
	if (!sk){
		cw_log(LOG_ERR,"Fatal: Can't allocate nl socket");
		return 0;
	}
	
	int rc = genl_connect(sk);
	if (rc){
		cw_log(LOG_ERR,"Fatal: Cannt connect nl socket, code %d",rc);
		nl_socket_free(sk);
		return 0;	
	}

	family_id = genl_ctrl_resolve(sk, "nl80211");

	if (family_id < 0){
		cw_log(LOG_ERR,"Fatal: Can't resolve nl80211, %d - %s",family_id,nl_geterror(family_id));
		nl_socket_free(sk);
		return 0;	
	}

	//attach a callback
	nl_socket_modify_cb(sk, NL_CB_VALID, NL_CB_CUSTOM, nlCallback,
			    NULL);

	return 1;
}



int init_radios()
{
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg){
		cw_log(LOG_ERR,"Fatal: No memory while initializing radios");
		return 0;
	}
	int flags = NLM_F_DUMP;

	/* setup the message */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, flags,
		    NL80211_CMD_GET_WIPHY, 0);

	int rc = nl_send_auto_complete(sk, msg);
	if (rc<0){
		cw_log(LOG_ERR,"Fatal: Can't send nl msg");
		return 0;
	}


	struct nl_cb bl_cb;	


	rc = nl_recvmsgs_default(sk);
	printf("IR RC: %d %s\n",rc,nl_geterror(rc));



}


void gr()
{
	if (!init())
		return;
	init_radios();
	make_if();

	start_ap(sk);

	sleep(1000);
	return;




	printf("Mak If\n");
	make_if();



	printf("made if\n");




	return;


	int ret;


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
//      NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifIndex);

//      NLA_PUT_FLAG(msg, NL80211_ATTR_SPLIT_WIPHY_DUMP);

	printf("NLAPIT\n");
	//send the messge (this frees it)
	ret = nl_send_auto_complete(sk, msg);
	printf("Send ret %d\n", ret);

	int nlr;

	//block for message to return
	nlr = nl_recvmsgs_default(sk);
	printf("NLR = %d\n", nlr);



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
