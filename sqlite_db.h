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

#ifndef _SQLITE_DB_H
#define _SQLITE_DB_H 1

#include <grace/str.h>
#include <grace/value.h>

#include <sqlite3.h>


class sqlite_db
{

	public:
					 sqlite_db (const string &dbname)
					 {
					 	_database = dbname;
					 }
					~sqlite_db (void)
					 {
					 }
	
		bool 		 connect (void);

		void		 close (void);
	
	
					 /// Execute a SQLite query
		value 		*dosqlite (const statstring &query);
	
	
					 // Get last error
		string		&getlasterror (void)
					 {
					 	return lasterror;
					 }
	
	protected:


		string		 _database;
		sqlite3 	*dbhandle;

		string		 lasterror;
		

};


#endif
