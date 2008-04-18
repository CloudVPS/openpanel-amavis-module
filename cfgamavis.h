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
	
	
		bool		 enabledomain (const string &domain);
		bool		 disabledomain (const string &domain);
		bool 		 removedomain (const string &domain);
	
	protected:
		string       _accessdb;
		string       _accessfile;
};

#endif
