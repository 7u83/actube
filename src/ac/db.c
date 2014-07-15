
#include <sqlite3.h>

#include "cw_log.h"

#include "conf.h"

static sqlite3 *handle;


const char * init_tables = "\
	CREATE TABLE IF NOT EXISTS acs (acid TEXT PRIMARY KEY, acname TEXT, lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS acips (acid TEXT,ip TEXT); \
	";

int db_init()
{
	const char * filename="ac.sqlite3";
	
	cw_log_debug0("Initi sqlite3 db: %s",filename);
	int rc = sqlite3_open(filename,&handle);
	if (rc)
	{
//		perror ("sqlite");	
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
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(handle, "INSERT INTO acs (acid,acname) VALUES (?,?);",-1,&stmt,0);
	printf ("RC = %d %p\n",rc,stmt);

	rc = sqlite3_bind_text(stmt,1,conf_acid,-1,SQLITE_STATIC);

	printf ("RC: %d\n",rc);

	sqlite3_bind_text(stmt,2,conf_acname,-1,SQLITE_STATIC);
	sqlite3_step(stmt);

	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=datetime('now') WHERE acid=?;",-1,&ping_stmt,0);
	rc = sqlite3_bind_text(ping_stmt,1,conf_acid,-1,SQLITE_STATIC);

//	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=99 WHERE acid=? ;",-1,&ping_stmt,0);
	printf("RCPin: %d\n",rc);


}
#include "conf.h"



int db_ping()
{
	int rc = sqlite3_step(ping_stmt);

	printf("Pingger\n");

	//int rc = sqlite3_prepare_v2(handle,cmd,-1,&ping_stmt,0);
	////printf ("Prepare rc: %d\n",rc);

	//printf(cmd,conf_acid);
	printf("\n");

//	int rc = sqlite3_exec(

}




void test_db()
{
// A prepered statement for fetching tables
	sqlite3_stmt *stmt;
//
// // Create a handle for database connection, create a pointer to sqlite3
	sqlite3 *handle;
	int rc = sqlite3_open("ac.sqlite3",&handle);

	if ( rc ) 
	{
		perror("sqlite");
		printf("Database failed\n");

	}

	printf ("DB RC: %i\n",rc);

	char cmd[100] = "xCREATE TABLE IF NOT EXISTS aclist (uname TEXT PRIMARY KEY,pass TEXT NOT NULL,activated INTEGER)";

	rc = sqlite3_exec(handle,cmd,0,0,0);

	printf("CT RC: %i\n",rc);
	const char *em = sqlite3_errmsg(handle);
	printf("ErrMsg: %s\n",em);
	return ;


}
