
#include <sqlite3.h>

#include "capwap/log.h"
#include "capwap/dbg.h"

#include "conf.h"

static sqlite3 *handle;


const char * init_tables = "\
	CREATE TABLE IF NOT EXISTS acs (acid TEXT PRIMARY KEY, acname TEXT, lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS acips (acid TEXT,ip TEXT); \
	CREATE TABLE IF NOT EXISTS wtps (wtpid TEXT PRIMARY KEY, wtp_name TEXT,lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS wtpprops (\
		wtpid TEXT,\
		rid INTEGER,\
		prop TEXT,\
		val TEXT,\
		upd INTEGER,\
		PRIMARY KEY(wtpid,rid,prop)\
	);\
	";

int db_init()
{
	const char * filename="ac.sqlite3";
	
	cw_dbg(DBG_INFO,"Initializing Sqlite3 DB: %s, SQLite3 Version %s",filename,SQLITE_VERSION);
	int rc = sqlite3_open(filename,&handle);
	if (rc != SQLITE_OK)
	{
		cw_log(LOG_ERR,"Error opening SQLite3 DB %s: %s",filename,sqlite3_errmsg(handle)); 
		return 0;
	}

	const char * cmd = init_tables;
	rc = sqlite3_exec(handle,cmd,0,0,0);
	if (rc)
	{
		const char *em = sqlite3_errmsg(handle);
		cw_log(LOG_ERR,"Error executing SQL \"%s\"\nSQL Error Message: %s",cmd, em);
		return 0;

	}

	return 1;
}


static sqlite3_stmt * ping_stmt;
static sqlite3_stmt * put_wtp_prop_stmt;
static sqlite3_stmt * get_tasks_stmt;



int db_start()
{
	cw_dbg(DBG_INFO,"Starting Sqlite3 DB");

	const char *sql="";

	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(handle, "INSERT INTO acs (acid,acname) VALUES (?,?);",-1,&stmt,0);
	if (rc)
		goto errX;

	rc = sqlite3_bind_text(stmt,1,conf_acid,-1,SQLITE_STATIC);

	rc = sqlite3_bind_text(stmt,2,conf_acname,-1,SQLITE_STATIC);

	sqlite3_step(stmt);

	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=datetime('now') WHERE acid=?;",-1,&ping_stmt,0);
	rc = sqlite3_bind_text(ping_stmt,1,conf_acid,-1,SQLITE_STATIC);


	/* Prepare statement to update a WTP property */
	sql = "INSERT OR REPLACE INTO wtpprops\
                (wtpid,rid,prop,val,upd)\
                VALUES (?,?,?,?,?)";

	rc = sqlite3_prepare_v2(handle, sql,-1, &put_wtp_prop_stmt,0);
	if (rc) 
		goto errX;


	
	sql = "SELECT * FROM wtpprops WHERE upd=0 AND wtpid=?";
	rc = sqlite3_prepare_v2(handle, sql,-1, &get_tasks_stmt,0);
	if (rc) 
		goto errX;

 


	return 1;

errX:
	cw_log(LOG_ERR,"Fatal: Can't start Sqlite3 DB, Error while executing '%s' - %d - %s",sql,rc,sqlite3_errmsg(handle));
	return 0;


}

void db_ping()
{
	int rc = sqlite3_step(ping_stmt);
	if (rc!=SQLITE_DONE){
		cw_log(LOG_ERR,"Error: Can't ping database, error code %d - %s",rc,sqlite3_errmsg(handle));
	}
}

void db_put_wtp_prop(const char *wtp_id,int rid, const char * prop,const char * val)
{
	int rc;

	sqlite3_reset(put_wtp_prop_stmt);
	sqlite3_clear_bindings(put_wtp_prop_stmt);

	if(sqlite3_bind_text(put_wtp_prop_stmt,1,wtp_id,-1,SQLITE_STATIC))
		goto errX;
	
	if(sqlite3_bind_int(put_wtp_prop_stmt,2,rid))
		goto errX;

	if (sqlite3_bind_text(put_wtp_prop_stmt,3,prop,-1,SQLITE_STATIC))
		goto errX;

	if (sqlite3_bind_text(put_wtp_prop_stmt,4,val,-1,SQLITE_STATIC))
		goto errX;

	if (sqlite3_bind_int(put_wtp_prop_stmt,5,0))
		goto errX;


	if (sqlite3_step(put_wtp_prop_stmt))
		goto errX;
	return;
errX:
	if (rc) {
		cw_log(LOG_ERR,"Can't update database with WTP props: %d - %s",
			rc,sqlite3_errmsg(handle));
	}

}


int db_get_tasks(const char * wtpid)
{
		
	sqlite3_reset(get_tasks_stmt);
	sqlite3_clear_bindings(get_tasks_stmt);

	if(sqlite3_bind_text(get_tasks_stmt,1,wtpid,-1,SQLITE_STATIC))
		goto errX;

	int rc;

	rc = sqlite3_step(get_tasks_stmt);
	if (rc == SQLITE_ROW) {
/*		DBGX("Have a rowi %s",sqlite3_column_text(get_tasks_stmt,0));
		DBGX("Have a rowi %s",sqlite3_column_text(get_tasks_stmt,1));
		DBGX("Have a rowi %s",sqlite3_column_text(get_tasks_stmt,2));
		DBGX("Have a rowi %s",sqlite3_column_text(get_tasks_stmt,3));
		DBGX("Have a rowi %s",sqlite3_column_text(get_tasks_stmt,4));
*/
	}

//	DBGX("The SQL RC: %d\n",rc);
	return 1;

errX:
	if (rc) {
		cw_log(LOG_ERR,"Can't get tasks: %d - %s",
			rc,sqlite3_errmsg(handle));
	}

	return 0;

}


