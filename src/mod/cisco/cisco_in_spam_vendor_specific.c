
#include "cw/action.h"
#include "cw/lw.h"
#include "cw/dbg.h"

int cisco_in_spam_vendor_specific(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	return lw_in_vendor_specific(conn,a,data+6,len-6,from);
	cw_dbg(DBG_X,"Cwin vendor spam");
	return 0;
}
