#ifndef CW_MAVL_H
#define CW_MAVL_H

#include "capwap/mavl.h"

extern void db_ping();
extern int db_init();
int db_start();
int db_get_tasks(struct conn * conn,const char * wtpid);
void db_put_wtp_prop(const char *wtp_id,const char * id,const char *sub_id,const char * val);
mavl_conststr_t db_get_update_tasks(struct conn * conn,const char * wtpid);


#endif


