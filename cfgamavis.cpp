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
bool cfgamavis::enabledomain (const string &domain, double treshold_subject, double treshold_kill)
{
	db4file DB;
	string  query;
			
	// Insert domain as a user rule
	query.printf (
		"REPLACE INTO users "
		"(priority, policy_id, email, fullname, local)"
		" VALUES "
		"(7, 5, '@%s', '', 'Y')", domain.str());
	
	delete dosqlite (query);
	
	query.crop();
	query.printf( "REPLACE INTO Policy (");
	query.printf( "id, policy_name, spam_tag2_level,  spam_kill_level");
	query.printf( ")VALUES(" );
	query.printf( "(SELECT id FROM users WHERE email = '@%s'),", domain.str());
	query.printf( "'Rules for %s',", domain.str());
	query.printf( "%f,%f)",  treshold_subject, treshold_kill);
	
	delete dosqlite (query);

// ::printf ("error: %s\n", getlasterror().str());		
	
    /*
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
	lasterror = "";*/
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
	string  query;
	query.printf( "DELETE FROM Policy WHERE policy_name = 'Rules for %s'", domain.str(), domain.str() );
	
	delete dosqlite (query);

	query.crop();
	query.printf ("DELETE FROM users WHERE email='@%s'", domain.str());
						  
	// Delete the current domain rule from table users					  
	delete dosqlite (query);
	 /*
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
	
	lasterror = ""; */
	return true;
}
