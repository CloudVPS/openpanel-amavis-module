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

#include <opencore/moduleapp.h>
#include <grace/file.h>
#include <grace/filesystem.h>
#include "amavismodule.h"
using namespace moderr;

APPOBJECT(amavismodule);




//  =========================================================================
/// Main method.
//  =========================================================================
int amavismodule::main (void)
{
	string conferr;
	
	// Add configuration watcher
	conf.addwatcher ("config", &amavismodule::confSystem);


   // Load will fail if watchers did not valiate.
    if (! conf.load ("openpanel.module.amavis", conferr))
    {   
        ferr.printf ("%% Error loading configuration: %s\n", conferr.str());
        return 1;
    } 		
     		
	caseselector (data["OpenCORE:Command"])
	{
		incaseof ("create") : 
			if (! checkconfig (data))
				return 0;

			if(! writeconfiguration (data))
				return false;
			break;

		incaseof ("update") :
			if (! checkconfig (data))
				return 0;

			if (! writeconfiguration (data))
				return 0;
			break;


		incaseof ("delete") :
			if (! checkconfig (data))
				return 0;

			if (! writeconfiguration (data))
				return 0;
			break;
		 	
		incaseof ("validate") : ;
		incaseof ("commit") : 
			if (! commitconfig ())
				return 0;
			break;
			
		incaseof ("revert") : 
			if (! revertconfig ())
				return 0;
			break;
			
		defaultcase:
			sendresult (err_command, 
						"Unsupported command");
			return 0;
	}

	// Can't quit when there is nothing done yet
	// authd.quit ();

	sendresult (moderr::ok, "");

	return 0;
}


//	=============================================
//	METHOD: amavismodule::commitconfig
//	=============================================
bool amavismodule::commitconfig (void)
{
//	// Send to authd: amavis save
//	if( authd.saveservice (conf["config"]["amavis:servicename"]))
//	{
//		sendresult( moderr::err_authdaemon, 
//					"Error saving service's configuration");
//		
//		return false;
//	}
	
	return true;
}


//	=============================================
//	METHOD: amavismodule::commitconfig
//	=============================================
bool amavismodule::revertconfig (void)
{
	
	return true;
}

//	=============================================
//	METHOD: amavismodule::readconfiguration
//	=============================================
bool amavismodule::readconfiguration (void)
{
	//
	// No read configuration supported by this module
	//
	
	return true;
}


//	=============================================
//	METHOD: amavismodule::readconfiguration
//	=============================================
bool amavismodule::deleteconfiguration (const value &v)
{
	// 
	// No configuration can be deleted, this module
	// only uses updates
	//

	return true;
}



//	==============================================
//	METHOD: amavismodule::writeconfiguration
//	==============================================
bool amavismodule::writeconfiguration (const value &v)
{
	// Create amavis database client object
	cfgamavis		 amavisdb (conf["config"]["amavis:dbfile"],
							   conf["config"]["postfix:accessfile"]);
	
	// Connect to the amavis sqlite db
	if (! amavisdb.connect ())
	{
		sendresult( moderr::err_unknown, 
					"Error opening amavis db file");		
		return false;	
	}

	string subdom = data["Mail"]["id"];
	string pdom = data["Domain"]["id"];
	subdom = subdom.left(subdom.strlen() - pdom.strlen() - 1);
	if (subdom != "")
		subdom.strcat(".");

	// Choose action pending on the requested command
	if (v["OpenCORE:Command"] == "delete")
	{
		amavisdb.removedomain (v["Mail"]["id"].sval());

		// remove the domain from the database
		// handle aliasdomains
		foreach(aliasdoms, data["Domain"]["Domain:Alias"])
		{
			string dom2;
			dom2 = subdom;
			dom2.strcat(aliasdoms["id"].sval());
			amavisdb.removedomain(dom2);
		}
	}
	else if ((v["OpenCORE:Command"] == "create") ||
			 (v["OpenCORE:Command"] == "update"))
	{
		// Enable spam and virus checking on 
		// the given domain
		if (v["Mail:Amavis"]["enabled"] == "true")
		{
			amavisdb.enabledomain (v["Mail"]["id"].sval());
			foreach(aliasdoms, data["Domain"]["Domain:Alias"])
			{
				string dom2;
				dom2 = subdom;
				dom2.strcat(aliasdoms["id"].sval());
				amavisdb.enabledomain(dom2);
			}
			
		}
		else
		{	
			amavisdb.disabledomain (v["Mail"]["id"].sval());	
			foreach(aliasdoms, data["Domain"]["Domain:Alias"])
			{
				string dom2;
				dom2 = subdom;
				dom2.strcat(aliasdoms["id"].sval());
				amavisdb.disabledomain(dom2);
			}
		}
	}
	
	
	if (v["OpenCORE:Command"] != "delete" && amavisdb.getlasterror().strlen() > 0)
	{
		string 	err = "Amavis database error: ";
			 	err.strcat (amavisdb.getlasterror());
		
		sendresult( moderr::err_unknown, err);
	
		return false;
	}
	
	
	// Close the amavis sqlite database
	amavisdb.close ();

	return true;
}



//  =========================================================================
/// amavismodule::installconfig
//  =========================================================================
bool amavismodule::installconfig (const value &v)
{
	// Unused
	// Configuration installation

	return true;
}



//  =========================================================================
/// amavismodule::reloadservices
//  =========================================================================
bool amavismodule::reloadservices (void)
{
	// Unused
	// Reload of amavis is not necessary.
	
	return true;
}



//  =========================================================================
/// amavismodule::checkconfig
//  =========================================================================
bool amavismodule::checkconfig (value &v)
{
	// Domain and Mail:Amavis class are required
	
	if ((! v.exists ("Domain")) || 
		(! v.exists ("Mail:Amavis")))
	{
		sendresult (err_value, 
					"Missing `Domain` or `Mail.Amavis class");
		return false;
	}
	
	// Domain should contain a domain name
	if (! v["Domain"].exists ("id"))
	{
		sendresult (err_value, 
					"Missing `id` field in `Domain`");
		return false;
	}
	
	if (v["OpenCORE:Command"] == "delete") return true;
	
	// Mail.Amavis should contain a file enabled which is true/false
	// empty field will be false by default
	if (! v["Mail:Amavis"].exists ("enabled"))
	{
		sendresult (err_value, 
					"Missing field `enabled` field in `Mail.Amavis`");
		return false;
	}
	
	
	// No errors during validation, 
	// main () will proceed request
	return true;
}



//  =========================================================================
/// Configuration watcher for the event log.
//  =========================================================================
bool amavismodule::confSystem (config::action act, keypath &kp,
                const value &nval, const value &oval)
{
	switch (act)
	{
		case config::isvalid:
			return true;

		case config::create:
			return true;		
	}

	return false;
}
