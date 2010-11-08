// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

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
