#include <errno.h>

#include <endian.h>



#include "wtpdrv.h"
#include "nlt.h"


#include "capwap/cw_log.h"
#include "capwap/radioinfo.h"
#include "capwap/sock.h"
#include "capwap/capwap_ieee80211.h"

int wpa_printf()
{
}

int wpa_hexdump()
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
#include <net/if_arp.h>


#include "dot11.h"
//#include "ieee802_11_defs.h"


struct radioinfo radioinfos[31];
static struct nl_sock * sk;


static struct apdata rd;


struct wiphydata {
	const char * name;

};


static struct wiphydata * wiphydata[31];

static int ack_handler(struct nl_msg *msg, void *arg)
{
printf ("Ack handler ------lllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
        int *err = arg;
        *err = 0;
        return NL_STOP;
}

static int finish_handler(struct nl_msg *msg, void *arg)
{
printf ("Finish handler ------lllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
        int *ret = arg;
        *ret = 0;
        return NL_SKIP;
}

static int error_handler(struct sockaddr_nl *nla, struct nlmsgerr *err,
                         void *arg)
{

printf ("Error handler ------lllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
        int *ret = arg;
        *ret = err->error;
printf("Err: %d\n",*ret);
        return NL_SKIP;
}




int nlt_send_and_recvmsg(struct nl_sock *sk, struct nl_msg *msg)
{
	volatile int err=NLE_NOMEM;
		

	struct nl_cb * cb = nl_socket_get_cb(sk);
	if (!cb)
		goto errX;

	cb = nl_cb_clone(cb);
	if (!cb)
		goto errX;

	
        err = nl_send_auto_complete(sk, msg);
        if (err < 0)
                goto errX;
	

        nl_cb_err(cb, NL_CB_CUSTOM, error_handler, &err);
        nl_cb_set(cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handler, &err);
        nl_cb_set(cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handler, &err);
	
	err=1;

        while (err==1) {
                int rc = nl_recvmsgs(sk, cb);
		printf("Weil rc %d\n",rc);
                if (rc < 0) {
			cw_log(LOG_ERR,"nl_recvmsg failed: %d %d",rc,nl_geterror(rc));
                }
        }


errX:
	printf ("Returning Err XXX %d\n",err);

	return err;
}



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





int get_ifhwaddr(const char *ifname, uint8_t *addr)
{
        struct ifreq ifr;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		return 0;


        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, ifname); //, IFNAMSIZ);
        if (ioctl(sockfd, SIOCGIFHWADDR, &ifr)) {
                return -1;
        }

        if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER) {
                return -1;
        }
        memcpy(addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
        return 0;
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

static int add_interface_data(struct nlattr* msgattribs[NL80211_ATTR_MAX+1])
{
	printf("Add interface_data\n");
	printf("--------------------------------------------------------------------------\n");
	int i;
	for (i=0; i<NL80211_ATTR_MAX; i++){
	


		if (msgattribs[i]){
			printf("Attrib %d, %s\n",i,nlt_get_attrname(i));




		}
	}


	if (msgattribs[NL80211_ATTR_IFINDEX]){
		int idx  = nla_get_u32( msgattribs[NL80211_ATTR_IFINDEX] );
		printf("IF index =  %d\n",idx);

		rd.idx=idx;

		
	}

	if (msgattribs[NL80211_ATTR_MAC]){

		uint8_t * d = nla_data( msgattribs[NL80211_ATTR_MAC] );
		memcpy (rd.mac,d,6);

		printf("Mac is: %s\n", sock_hwaddr2str(rd.mac,6));
		
	}


	if (msgattribs[NL80211_ATTR_IFNAME]){
		printf("IFNAME = %s\n",(uint8_t*)nla_data(msgattribs[NL80211_ATTR_IFNAME]));
	}

	if (msgattribs[NL80211_ATTR_IFTYPE]){
		printf("TYPECHEN = %d\n",(uint8_t*)nla_get_u32(msgattribs[NL80211_ATTR_IFTYPE]));
	}


	return 1;
}


static int add_wiphy_data(struct nlattr* msgattribs[NL80211_ATTR_MAX+1])
{
//	const char * name;
	if (!msgattribs[NL80211_ATTR_WIPHY]){
		printf("Error, on index\n");
		return 0;
	}


	int index = nla_get_u32( msgattribs[NL80211_ATTR_WIPHY] );


	struct wiphydata * w = wiphydata[index];

	if (!w) {
		w = malloc (sizeof(struct wiphydata));
		memset(w,0,sizeof(struct wiphydata));
		wiphydata[index]=w;
	}


	if (msgattribs[NL80211_ATTR_WIPHY_NAME] && !w->name){
		w->name = strdup( nla_get_string(msgattribs[NL80211_ATTR_WIPHY_NAME]));
	}



	printf ("Got data for wiphy index %d\n",index);
	return 0;	


	if (msgattribs[NL80211_ATTR_WIPHY_BANDS]){
		printf("Yea band\n");	
//		struct nlattr *nla=msgattribs[NL80211_ATTR_WIPHY_BANDS];
//		struct nla_attr * cnla;
//		int rem_nla;

		/* TODO parse bands */
/*		nla_for_each_nested(cnla, nla, rem_nla) {
			int band = nla_get_u16(cnla);
			printf("Band found: %d\n",band);
		}
*/



	

	}

	if (msgattribs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD]){
		int tr  = nla_get_u32( msgattribs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD] );
		printf("Frag threshold: %d\n",tr);
		
	}

	if (msgattribs[NL80211_ATTR_SUPPORTED_IFTYPES]){
		printf("IFTYPES -.\n");

//		struct nlattr *nla=msgattribs[NL80211_ATTR_SUPPORTED_IFTYPES];
//		struct nla_attr * cnla;
//		int rem_nla;

		/* TODO parse bands */
/*		nla_for_each_nested(cnla, nla, rem_nla) {
			int t = nla_type(cnla);
			printf("IFTYPE: %d\n",t);
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
		case NL80211_CMD_NEW_INTERFACE:
			add_interface_data(msgattribs);

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


struct ifindex_cb_data{
	const char * ifname;
	int index;
	int type;
};

static int get_ifindex_cb(struct nl_msg *msg, void *arg)
{
	struct ifindex_cb_data * cb_data = (struct ifindex_cb_data *)arg;

	printf("Yeaaaaaaaaaa! The ifindex callback is here\n");
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

	if (cmd == NL80211_CMD_NEW_INTERFACE){
		if (msgattribs[NL80211_ATTR_IFNAME]){

			printf ("IFNAME = %s\n",nla_get_string(msgattribs[NL80211_ATTR_IFNAME]));

			cb_data->index = nla_get_u32(msgattribs[NL80211_ATTR_IFINDEX]);
			cb_data->type = nla_get_u32(msgattribs[NL80211_ATTR_IFTYPE]);
			

		}

	}

}

int get_ifindex(const char *ifname)
{
	int index = -1;
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return -1;

	int flags = NLM_F_DUMP;


	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, flags,
		    NL80211_CMD_GET_INTERFACE, 0);

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, 0);
	int rc = nl_send_auto_complete(sk, msg);


	printf ("AC C %d\n",rc);
	

	struct ifindex_cb_data cb_data;

	cb_data.ifname=ifname;
	cb_data.index=-1;


	int err;

	struct nl_bc * nl_cb = nl_cb_alloc(NL_CB_CUSTOM);
	nl_cb_set(nl_cb,NL_CB_VALID,NL_CB_CUSTOM, get_ifindex_cb,&cb_data);
        nl_cb_err(nl_cb,NL_CB_CUSTOM, error_handler, &err);
	

	int nlr;
	do {
		nl_recvmsgs(sk,nl_cb);
	}while(cb_data.index==-1);

	//int nlr = nl_recvmsgs_default(sk);
	cw_log(LOG_ERR,"iGet if index: Make if %d - %s",nlr,nl_geterror(nlr));


	if (cb_data.index!=-1){
		index=cb_data.index;
	}

	printf("The mode %d\n",cb_data.type);

printf ("TTTTTTTTTTTTTTTHe index: %d\n",index);
      nla_put_failure:
	nlmsg_free(msg);
	return index;

}

int del_if(const char * name)
{
	int index = get_ifindex(name);
	if (index==-1)
		return -1;

	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return 0;

	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, 0,
		    NL80211_CMD_DEL_INTERFACE, 0);

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, index);

	int nlr = nl_send_sync(sk, msg);
	if (nlr){
		cw_log(LOG_ERR,"Error deleting interface %s(%d): %d - %s",name,index,nlr,nl_geterror(nlr));
		return nlr;
	}

	return 0;	


      nla_put_failure:
	nlmsg_free(msg);
	return 1;

}


int create_interface(const char * ifname)
{
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return 0;

	genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST | NLM_F_REPLACE,
		    NL80211_CMD_NEW_INTERFACE, 0);


	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, NL80211_IFTYPE_AP);
	NLA_PUT_STRING(msg, NL80211_ATTR_IFNAME, ifname);

	int nlr = nl_send_sync(sk,msg);
	if (nlr){
		cw_log(LOG_ERR,"Fatal: Make if %d - %s",nlr,nl_geterror(nlr));
	}

	sleep(1);
	return 0;

      nla_put_failure:
	nlmsg_free(msg);
	return 1;
}


int start_ap(struct nl_sock *sk)
{








	/* allocate a message */
	struct nl_msg *msg = nlmsg_alloc();
	if (!msg)
		return 0;



	/* init message */
	genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST,
		    NL80211_CMD_START_AP, 0);
//	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, 0,
//		    NL80211_CMD_SET_BEACON, 0);



/*
	struct dot11_mgmt *head = NULL;

	head = malloc(256);
	uint8_t * tail;

	head->frame_control = htole16 (DOT11_FTYPE_MGMT | DOT11_STYPE_BEACON) ; 

	head->duration = htole16(0);

	memset(head->da, 0xff, sizeof(head->da));
	memcpy (head->sa , rd.mac,6);
	memcpy (head->bssid , rd.mac,6);

	head->u.beacon.beacon_int=htole16(100);
	head->u.beacon.capab_info=0;
	memset (head->u.beacon.timestamp,0,8);

	

	int hs = sizeof(struct dot11_mgmt_head);
	printf("HEAD SIZE: %d\n",hs);

	hs += sizeof( head->u.beacon);
	printf("Head size is now %d\n",hs);
*/

	struct beacon_data bd;

	const char *ssid = "xTatort88";

	struct apdata * ap  = &rd;
	ap->ssid=ssid;

printf("Get Beacon Data \n");
	dot11_get_beacon_data(ap,&bd);
printf("Got Beaqcon Fata\n");
printf("headlen %d\n",bd.head_len);

	NLA_PUT(msg, NL80211_ATTR_BEACON_HEAD, bd.head_len, bd.head);


printf ("Coter\n");
	{
		for (int i=0; i<bd.head_len; i++){
			printf("%02X ",bd.head[i]);
		}
	
	}


printf("Put message 1\n");

/*
	tail = (uint8_t*)(head)+hs;
	uint8_t * pos = tail;
printf("Put message 1 posss\n");

	*pos = WLAN_EID_SSID;
	pos++;

	*pos = strlen(ssid);
	pos++;

printf ("Memcpy %d %d %d\n",(void*)pos,(void*)head,(void*)pos-(void*)head);
	memcpy(pos,ssid,strlen(ssid));
printf ("Memcoy done\n");

	pos+=strlen(ssid);

	int tl = pos-tail;
	printf ("Tail len %d\n",tl);

	NLA_PUT(msg, NL80211_ATTR_BEACON_TAIL, tl, tail);
*/


printf ("The index: %d\n",rd.idx);

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, 6);//rd.idx);
	NLA_PUT_U32(msg, NL80211_ATTR_BEACON_INTERVAL, 100);
//	NLA_PUT_U32(msg, NL80211_ATTR_HIDDEN_SSID, NL80211_HIDDEN_SSID_NOT_IN_USE);


printf("The SSID: %s\n",ssid);

//	NLA_PUT(msg, NL80211_ATTR_SSID, strlen(ssid), ssid);

//	NLA_PUT(msg, NL80211_ATTR_MAC_ADDRESS,6,rd.mac);


	NLA_PUT_U32(msg, NL80211_ATTR_DTIM_PERIOD, 2);

/*	NLA_PUT_U32(msg, NL80211_ATTR_CIPHER_SUITES_PAIRWISE, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_CIPHER_SUITE_GROUP, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_AKM_SUITES, 0);
	NLA_PUT_U32(msg, NL80211_ATTR_AUTH_TYPE, NL80211_AUTHTYPE_OPEN_SYSTEM);
*/

//	NLA_PUT_U32(msg, NL80211_ATTR_WPA_VERSIONS, 0); //NL80211_WPA_VERSION_2);
//	NLA_PUT_FLAG(msg, NL80211_ATTR_PRIVACY, 0);

        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, 2462);

//      nla_put(msg,NL80211_ATTR_SSID,ssid,strlen(ssid));
	printf("Sot ssid\n");


////	int ret = nl_send_auto_complete(sk, msg);
//	printf("AP IF Send ret %d\n", ret);

//	int nlr = nl_recvmsgs_default(sk);


//	int nlr = nlt_send_and_recvmsg(sk,msg);
	int nlr = nl_send_sync(sk,msg);

	printf("Start AP NLR = %d\n", nlr);
	cw_log(LOG_ERR,"Fatal: Setup AP  %d - %s",nlr,nl_geterror(nlr));

/*
	msg = nlmsg_alloc();

	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST,
		    NL80211_CMD_SET_CHANNEL, 0);

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, 8);
        NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, 2412);
	int rc = nl_send_auto_complete(sk, msg);

	printf ("Set Channel RC: %d\n",rc);

	nlr = nl_recvmsgs_default(sk);
	printf("Start Channel AP NLR = %d\n", nlr);


*/






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
//	nl_socket_modify_cb(sk, NL_CB_VALID, NL_CB_CUSTOM, nlCallback,
//			    NULL);

	return 1;
}



int init_radios()
{
	memset (wiphydata,0,sizeof(wiphydata));

	struct nl_msg *msg = nlmsg_alloc();
	if (!msg){
		cw_log(LOG_ERR,"Fatal: No memory while initializing radios");
		return 0;
	}
	int flags = NLM_F_DUMP;

	/* setup message to enumerate wiphy */
	genlmsg_put(msg, 0, NL_AUTO_SEQ, family_id, 0, flags,
		    NL80211_CMD_GET_WIPHY, 0);

	int rc = nl_send_auto_complete(sk, msg);
	if (rc<0){
		cw_log(LOG_ERR,"Fatal: Can't send nl msg %d %s",rc,nl_geterror(rc));
		goto nla_put_failure;
	}

	rc = nl_recvmsgs_default(sk);
	if (rc){

		cw_log(LOG_ERR,"Error enumerating wiphy: %d %s",rc,nl_geterror(rc));
		goto nla_put_failure;
	}


      nla_put_failure:
	nlmsg_free(msg);
	return 1;

}


int set_iftype(int ifindex)
{
	struct nl_msg *msg = nlmsg_alloc();

	if (!msg) {
		return 0;
	}

	genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, family_id, 0, NLM_F_REQUEST,
		    NL80211_CMD_SET_INTERFACE, 0);



	NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, NL80211_IFTYPE_AP);

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifindex);

	int nlr = nl_send_sync(sk,msg);

	if (nlr){
		cw_log(LOG_ERR,"Fatal: Set Mode %d - %s",nlr,nl_geterror(nlr));
	}

	return 0;


      nla_put_failure:
	nlmsg_free(msg);
	return 1;

}


int gr()
{

	if (!init())
		return 0;

printf("Calling get wiphy info\n");
nlt_get_wiphy_list(sk);

exit(0);

printf("Calling get info\n");

struct nlt_ifinfo ifinfo;
nlt_init_ifinfo(&ifinfo);
ifinfo.ifname="wlan0";

nlt_get_ifinfo(sk,&ifinfo);

printf("Have ifinfo\n");


exit(0);


	const char * ifname = "wtpi1";
	
	int ifindex = get_ifindex(ifname);
	if (ifindex == -1){
		printf("Must create\n");
		create_interface(ifname);

	}

	ifindex = get_ifindex(ifname);
	if (ifindex < 0 ){
		cw_log("Error getting interface %s\n",ifname);
		return 0;
	}
		
	set_iftype(ifindex);
	exit(0);


 
//	init_radios();

//	int index = get_ifindex("wlan1");

//	printf("Index is: %d\n",index);


	printf("--- del ---\n");
	del_if("tube");
	printf("--- make ---\n");
//	make_if("tube");
	printf("--- get ---\n");

	int id = get_ifindex("tube");

sleep(1);	
	printf ("ID = %d\n",id);
	//interface_up("tube");

//	set_mode(id);
	printf("setted mode\n");


exit(0);
	printf("Now starting the AP\n");

	start_ap(sk);

	printf("Started the AP\n");

	sleep(1000);

return 0;


	return 0 ;




	printf("Mak If\n");



	printf("made if\n");




	return 0 ;


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
	return 2;
}



int wtpdrv_get_radioinfo(int rid,struct radioinfo * radioinfo)
{

	radioinfo->rid=rid;
	radioinfo->type|=CW_IEEE80211_RADIO_TYPE_B; //CWRADIO_TYPE_N;

/*
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
	
*/

}


