#include <errno.h>

#ifdef __FreeBSD__
	#include "sys/endian.h"
#else
	#include <endian.h>
#endif



//#include "wtpdrv.h"
//#include "nlt.h"


#include "cw/log.h"
#include "cw/radioinfo.h"
#include "cw/sock.h"
//#include "cw/capwap_80211.h"
#include "cw/dbg.h"

/*
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
*/

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

/*
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
*/


/*
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


*/

