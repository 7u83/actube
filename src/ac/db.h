#ifndef CW_MAVL_H
#define CW_MAVL_H

#include "cw/mavl.h"

extern void db_ping();
extern void db_ping_wtp(const char *wtpid,const char *acid);

extern int db_init();
int db_start();
int db_get_tasks(struct conn * conn,const char * wtpid);
void db_put_wtp_prop(const char *wtp_id,const char * id,const char *sub_id,const char * val);
mavl_conststr_t db_get_update_tasks(struct conn * conn,const char * wtpid);

void db_put_radio_prop(const char *wtp_id,const char *rid, const char * key,const char *sub_key,const char * val);
extern mavl_conststr_t db_get_radio_tasks(struct conn * conn,const char * wtpid);


#endif


