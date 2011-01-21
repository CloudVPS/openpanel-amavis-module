// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#ifndef _CFGAMAVIS_H
#define _CFGAMAVIS_H 1

#include "sqlite_db.h"

#include <grace/str.h>


class cfgamavis : public sqlite_db
{
	public:
	
					 cfgamavis (const string &dbname, const string &accessdbname)
					 			: sqlite_db (dbname)
					 {
						_accessfile = accessdbname;
						_accessdb.printf("%s.db", accessdbname.str());
					 }
					~cfgamavis ()
					 {
					 }
	
	
		bool		 enabledomain (const string &domain, double treshold_subject, double treshold_kill);
		bool		 disabledomain (const string &domain);
		bool 		 removedomain (const string &domain);
	
	protected:
		string       _accessdb;
		string       _accessfile;
};

#endif
