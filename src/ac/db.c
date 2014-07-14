
#include <sqlite3.h>

#include "cw_log.h"

static sqlite3 *handle;


const char * init_tables = "\
	CREATE TABLE IF NOT EXISTS acs (acid TEXT PRIMARY KEY, TIMESTAMP lastseen); \
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

int db_start()
{
	const char * cmd = "REPLACE INTO acs ";
}
#include "conf.h"

int db_ping()
{
	printf("Pingger\n");
	const char * cmd = "UPDATE acs SET lastseen=datetime('now') WHERE acid='%s'";
	printf(cmd,conf_acid);
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
