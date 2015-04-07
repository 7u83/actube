
#include <sqlite3.h>

#include "capwap/cw_log.h"

#include "conf.h"

static sqlite3 *handle;


const char * init_tables = "\
	CREATE TABLE IF NOT EXISTS acs (acid TEXT PRIMARY KEY, acname TEXT, lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS acips (acid TEXT,ip TEXT); \
	CREATE TABLE IF NOT EXISTS wtps (wtpid TEXT, wtp_name TEXT); \
	";

int db_init()
{
	const char * filename="ac.sqlite3";
	
	cw_dbg(DBG_CW_INFO,"Init sqlite3 db: %s",filename);
	int rc = sqlite3_open(filename,&handle);
	if (rc)
	{

		return 0;

	}

	const char * cmd = init_tables;
	rc = sqlite3_exec(handle,cmd,0,0,0);
	if (rc)
	{
		const char *em = sqlite3_errmsg(handle);
		cw_log(LOG_ERR,"Error executing SQL \"%s\" - Error msg: %s",cmd, em);
		return 0;

	}

	return 1;
}


static sqlite3_stmt * ping_stmt;


int db_start()
{
	cw_dbg(DBG_CW_INFO,"Start sqlite3 db");

	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(handle, "INSERT INTO acs (acid,acname) VALUES (?,?);",-1,&stmt,0);
	if (rc)
		goto errX;

	rc = sqlite3_bind_text(stmt,1,conf_acid,-1,SQLITE_STATIC);

	rc = sqlite3_bind_text(stmt,2,conf_acname,-1,SQLITE_STATIC);

	sqlite3_step(stmt);

	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=datetime('now') WHERE acid=?;",-1,&ping_stmt,0);
	rc = sqlite3_bind_text(ping_stmt,1,conf_acid,-1,SQLITE_STATIC);

//	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=99 WHERE acid=? ;",-1,&ping_stmt,0);
	return 1;

errX:
	cw_log(LOG_ERR,"Fatal: Can't start sqlite3 db, error %d",rc);
	return 0;


}


void db_ping()
{
	int rc = sqlite3_step(ping_stmt);
	if (rc!=SQLITE_DONE){
		cw_log(LOG_ERR,"Error: Can't ping database, error code %d",rc);
	}
}



