

#include "capwap.h"
#include "dbg.h"
#include "cw_log.h"

void cw_dbg_missing_mand(int level,struct conn *conn,cw_action_in_t ** ml,int n,cw_action_in_t *a)
{
	if (!cw_dbg_is_level(DBG_ELEM) || n==0)
		return;

	char buffer[2000];
	char *p=buffer;
	int i;
	char *delim="";
	for(i=0; i<n; i++) {
		p+=sprintf(p,"%s",delim); delim=", ";
		p+=sprintf(p,"%s",cw_strelemp(conn->actions,ml[i]->elem_id));
		
	}

	cw_dbg(DBG_ELEM,"Missing mandatory elements: [%s]",buffer);
}
