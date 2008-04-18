// --------------------------------------------------------------------------
// OpenPanel - The Open Source Control Panel
// Copyright (c) 2006-2007 PanelSix
//
// This software and its source code are subject to version 2 of the
// GNU General Public License. Please be aware that use of the OpenPanel
// and PanelSix trademarks and the IconBase.com iconset may be subject
// to additional restrictions. For more information on these restrictions
// and for a copy of version 2 of the GNU General Public License, please
// visit the Legal and Privacy Information section of the OpenPanel
// website on http://www.openpanel.com/
// --------------------------------------------------------------------------

#include "sqlite_db.h"


//	===========================================================================
///	METHOD: sqlite_db::connect
//	===========================================================================
bool sqlite_db::connect (void)
{
	if (sqlite3_open (_database.str(), 
					  &dbhandle) != SQLITE_OK)
	{
		// FIXME: need consistent reporting upwards
		lasterror.crop   ();
		lasterror.printf ("sqlite3_open(%s) failed: %s", 
						  _database.str(), 
						  sqlite3_errmsg(dbhandle));
						  
		sqlite3_close (dbhandle);
		dbhandle = NULL;
		
		return false;
	}
	
	return true;
}



//	===========================================================================
///	METHOD: sqlite_db::close
//	===========================================================================
void sqlite_db::close ()
{
	// close dbhandle, free associated resources
	sqlite3_close (dbhandle);
}



//	===========================================================================
///	METHOD: sqlite_db::dosqlite
//	===========================================================================
value *sqlite_db::dosqlite (const statstring &query)
{
	returnclass (value) res retain;
	sqlite3_stmt *qhandle;
	int qres;
	
	
	if (sqlite3_prepare (dbhandle, 
						 query.str(), 
						 -1, 
						 &qhandle, 
						 0) 
						 != SQLITE_OK)
	{
		lasterror.crop();
		lasterror.printf ("sqlite3_prepare(%s) failed: %s", 
						  query.str(), 
						  sqlite3_errmsg (dbhandle));
						  
		return &res; // empty
	}
	
	bool done=false;
	while ((qres = sqlite3_step(qhandle)) && !done)
	{
		switch(qres)
		{
			case SQLITE_BUSY:
				sleep(1);
				continue;
				break;
				
			case SQLITE_MISUSE:  // fallthrough
			case SQLITE_ERROR:
				lasterror = sqlite3_errmsg(dbhandle);

			case SQLITE_DONE:
				done=true;
				break;
				
			case SQLITE_ROW:
				res["columncount"] = sqlite3_column_count(qhandle);
				
				if (res["columncount"] == 0)
				{
					res["rowschanged"] = sqlite3_changes(dbhandle);
					done=true;
					break;
				}
				
				value row;
				
				for (int i=0; i < res["columncount"].ival(); i++)
				{
					row[sqlite3_column_name(qhandle, i)] = 
							(const char *) sqlite3_column_text(qhandle, i);
				}
				
				res["rows"].newval()=row;
				break;
		}
	}
	
	if (! done)
	{
		// we should never get here!
		// FIXME: rant and rave harder
		lasterror.crop();
		lasterror.printf ("dbmanager::dosqlite(%s): pigs fly :(", 
						  query.cval());
						  
		res.clear();
		return &res;
	}
	
	if (sqlite3_finalize (qhandle) != SQLITE_OK)
	{				   
		lasterror.crop();
		lasterror.printf ("sqlite3_finalize(%s) failed: %s", 
						 query.cval(), 
						 sqlite3_errmsg (dbhandle));
		res.clear();
		return &res;
	}
	
	res["insertid"] = sqlite3_last_insert_rowid (dbhandle);
			   
	return &res;
}
