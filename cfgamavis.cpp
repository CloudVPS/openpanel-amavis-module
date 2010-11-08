// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/


#include <dbfile/db4file.h>

#include "cfgamavis.h"

// TODO: use escapeforsql & friends instead of simple printf/

//	===========================================================================
///	METHOD: cfgamavis::enabledomain
//	===========================================================================
bool cfgamavis::enabledomain (const string &domain)
{
	db4file DB;
	string  query;
			
	string 	domainmask;
	domainmask.printf ("@%s", domain.str());
			
	// Delete the current domain rule from table users					  
	delete dosqlite (query);

// ::printf ("error: %s\n", getlasterror().str());	
	
	query.crop 	 ();
	query.printf ("REPLACE INTO users VALUES "
				  "( NULL, 7, 5, '%s', ", domainmask.str());
	query.printf ("'', 'Y')");
	
	// Insert domain
	delete dosqlite (query);

// ::printf ("error: %s\n", getlasterror().str());		
	
	DB.setencoding (dbfile::flat);
    
    // Open the public database file. Bail out on failure.
    if (! DB.open (_accessdb))
    {
			lasterror = "failed to open accessdb";
            DB.close ();
            return false;
    }
  
	DB.db[domain.str()] = (string) "FILTER smtp-amavis:[127.0.0.1]:10024";
	DB.commit ();
	DB.close ();
	lasterror = "";
	return true;
}



//	===========================================================================
///	METHOD: cfgamavis::disabledomain
//	===========================================================================
bool cfgamavis::disabledomain (const string &domain)
{
	return removedomain(domain);
}

//	===========================================================================
///	METHOD: cfgamavis::removedomain
//	===========================================================================
bool cfgamavis::removedomain (const string &domain)
{
	db4file DB;
	string 	domainmask;
	domainmask.printf ("@%s", domain.str());
			
	string  query;
	query.printf ("DELETE FROM users WHERE email='%s'", 
						  domainmask.str());
						  
	// Delete the current domain rule from table users					  
	delete dosqlite (query);
	
	DB.setencoding (dbfile::flat);
    
    // Open the public database file. Bail out on failure.
    if (! DB.open (_accessdb))
    {
			lasterror = "failed to open accessdb";
	        DB.close ();
            return false;
    }
  
	DB.rmval (domain);
	DB.commit ();
	DB.close ();
	
	lasterror = "";
	return true;
}
