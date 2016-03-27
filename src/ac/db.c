
#include <sqlite3.h>

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/capwap_items.h"
#include "cw/conn.h"
#include "cw/item.h"

#include "conf.h"

static sqlite3 *handle;


const char * init_tables = "\
	CREATE TABLE IF NOT EXISTS acs (acid TEXT PRIMARY KEY, acname TEXT, lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS acips (acid TEXT,ip TEXT); \
	CREATE TABLE IF NOT EXISTS wtps (wtpid TEXT PRIMARY KEY, acid TEXT,lastseen TIMESTAMP); \
	CREATE TABLE IF NOT EXISTS wtpprops (\
		wtpid TEXT NOT NULL,\
		id TEXT NOT NULL,\
		sub_id TEXT NOT NULL,\
		val TEXT,\
		upd INTEGER,\
		PRIMARY KEY(wtpid,id,sub_id)\
	);\
	CREATE TABLE IF NOT EXISTS wlans (wlanid INTEGER PRIMARY KEY);\
	CREATE TABLE IF NOT EXISTS wlanprops (wlanid INTEGER, id TEXT NOT NULL, val TEXT, PRIMARY KEY(wlanid,id));\
	";
	



int db_init()
{

	int rc;
	const char * filename="ac.sqlite3";
	cw_dbg(DBG_INFO,"Initializing Sqlite3 DB: %s, SQLite3 Version %s",filename,SQLITE_VERSION);

	rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
	if (rc!=SQLITE_OK){
		cw_log(LOG_ERR,"Error configuring SQLite3: %s",sqlite3_errmsg(handle)); 
		return 0;
	} 
	
	rc = sqlite3_initialize();
	if (rc!=SQLITE_OK){
		cw_log(LOG_ERR,"Error initializing SQLite3 DB : %s",sqlite3_errmsg(handle)); 
		return 0;
	} 
	

	
	rc = sqlite3_open(filename,&handle);
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

static sqlite3_stmt * stmt_ping_wtp;


int db_start()
{
	cw_dbg(DBG_INFO,"Starting Sqlite3 DB");

	const char *sql="";

	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(handle, "INSERT OR REPLACE INTO acs (acid,acname) VALUES (?,?);",-1,&stmt,0);
	if (rc)
		goto errX;

	rc = sqlite3_bind_text(stmt,1,conf_acid,-1,SQLITE_STATIC);

	rc = sqlite3_bind_text(stmt,2,conf_acname,-1,SQLITE_STATIC);

	sqlite3_step(stmt);

	rc = sqlite3_prepare_v2(handle, "UPDATE acs SET lastseen=datetime('now') WHERE acid=?;",-1,&ping_stmt,0);
	rc = sqlite3_bind_text(ping_stmt,1,conf_acid,-1,SQLITE_STATIC);


	/* Prepare statement to update a WTP property */
	sql = "INSERT OR REPLACE INTO wtpprops\
                (wtpid,id,sub_id,val,upd)\
                VALUES (?,?,?,?,?)";

	rc = sqlite3_prepare_v2(handle, sql,-1, &put_wtp_prop_stmt,0);
	if (rc) 
		goto errX;


	/* Prepare WTP ping statement */
	sql = "INSERT OR REPLACE INTO wtps  (wtpid,acid,lastseen) VALUES(?,?,datetime('now'))";
	rc = sqlite3_prepare_v2(handle, sql,-1, &stmt_ping_wtp,0);
	if (rc) 
		goto errX;


	
	sql = "SELECT wtpid,id,sub_id,val FROM wtpprops WHERE upd>0 AND wtpid=?";
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


void db_ping_wtp(const char *wtpid,const char *acid)
{
	int rc=0;
	sqlite3_reset(stmt_ping_wtp);
	sqlite3_clear_bindings(stmt_ping_wtp);
	if(sqlite3_bind_text(stmt_ping_wtp,1,wtpid,-1,SQLITE_STATIC))
		goto errX;
	
	if(sqlite3_bind_text(stmt_ping_wtp,2,acid,-1,SQLITE_STATIC))
		goto errX;

	rc = sqlite3_step(stmt_ping_wtp);
errX:
	if (rc!=SQLITE_DONE) {
		cw_log(LOG_ERR,"Can't ping database for WTP: %d - %s",
			rc,sqlite3_errmsg(handle));
	}



}

void db_put_wtp_prop(const char *wtp_id,const char * id,const char *sub_id,const char * val)
{
	int rc=0;

//	DBGX("Putting %s/%s:%s",id,sub_id,val);

	sqlite3_reset(put_wtp_prop_stmt);
	sqlite3_clear_bindings(put_wtp_prop_stmt);

	if(sqlite3_bind_text(put_wtp_prop_stmt,1,wtp_id,-1,SQLITE_STATIC))
		goto errX;
	
	if(sqlite3_bind_text(put_wtp_prop_stmt,2,id,-1,SQLITE_STATIC))
		goto errX;

	if (!sub_id) 
		sub_id=CW_ITEM_NONE;
	

	if (sqlite3_bind_text(put_wtp_prop_stmt,3,sub_id,-1,SQLITE_STATIC))
		goto errX;

	if (sqlite3_bind_text(put_wtp_prop_stmt,4,val,-1,SQLITE_STATIC))
		goto errX;

	if (sqlite3_bind_int(put_wtp_prop_stmt,5,0))
		goto errX;

//	cw_dbg(DBG_X,"Her I am already, next is step");

	rc = sqlite3_step(put_wtp_prop_stmt);
	if (rc != SQLITE_DONE)
		goto errX;


//	cw_dbg(DBG_X,"SQL schould be fine");

	return;
errX:
//	cw_dbg (DBG_X, "Iam on err %d\n",rc);


	if (rc) {
		cw_log(LOG_ERR,"Can't update database with WTP props: %d - %s",
			rc,sqlite3_errmsg(handle));
	}

}


mavl_conststr_t db_get_update_tasks(struct conn * conn,const char * wtpid)
{
		
	sqlite3_reset(get_tasks_stmt);
	sqlite3_clear_bindings(get_tasks_stmt);

	mavl_conststr_t r = mavl_create_conststr();
	if (!r)
		return NULL;


	int rc=0;


	if(sqlite3_bind_text(get_tasks_stmt,1,wtpid,-1,SQLITE_STATIC))
		goto errX;




	//rc = sqlite3_step(get_tasks_stmt);
	while (SQLITE_ROW == sqlite3_step(get_tasks_stmt)) {

		int ii;
		//DBGX("-----------------------------------------------------","");
		for (ii=0; ii<5; ii++){

			//DBGX("CVALL: %s",(const char*)sqlite3_column_text(get_tasks_stmt,ii));


		}

		const char *id =  (const char*)sqlite3_column_text(get_tasks_stmt,1);
		if (!id) {
			//DBGX("::::::::::::::::::::::::::::::::::::::::::::::::::::NULL ID!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!","");
			continue;
		}

		const char *sub_id =  (const char*)sqlite3_column_text(get_tasks_stmt,2);

		const char *val =  (const char*)sqlite3_column_text(get_tasks_stmt,3);

		//DBGX("ID: (%s), SubID (%s), Val (%s)",id,sub_id,val);
	
		const struct cw_itemdef * cwi = cw_itemdef_get(conn->actions->items,id,sub_id);
		if (!cwi) {
			//DBGX("Not item definition found for: %s/%s",id,sub_id);
			continue;	
		}

		//uint8_t data[2048];

		if (!cwi->type->from_str) {
			cw_log(LOG_ERR,"Can't convert from string %s/%s - No method defined.",id,sub_id);
			continue;
		}


		mbag_item_t * i = cwi->type->from_str(val);
		i->id=cwi->id;

		mbag_set(conn->outgoing,i);
	
		mavl_add(r,(void*)cwi->id);
	}

	if (r->count)
		return r;

	
	mavl_destroy(r);
	return NULL;


errX:
	if (rc) {
		cw_log(LOG_ERR,"Can't get tasks: %d - %s",
			rc,sqlite3_errmsg(handle));
	}

	if (r)
		mavl_destroy(r);
		

	return NULL;

}




