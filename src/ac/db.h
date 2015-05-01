extern void db_ping();
extern int db_init();
int db_start();
void db_put_wtp_prop(const char *wtp_id,int rid, const char * prop,const char * val);
int db_get_tasks(struct conn * conn,const char * wtpid);



