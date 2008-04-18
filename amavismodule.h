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

#ifndef _amavismodule_H
#define _amavismodule_H 1

#include <moduleapp.h>
#include <grace/system.h>
#include <grace/configdb.h>

#include "cfgamavis.h"

typedef configdb<class amavismodule> appconfig;

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class amavismodule : public moduleapp
{
public:
			 // ===============================================
			 /// Constructor
			 // ===============================================
		 	 amavismodule (void) :
				moduleapp ("openpanel.module.amavis"),
				conf (this)
			 {
			 }
			 
			 // ===============================================
			 /// Destructor
			 // ===============================================
			~amavismodule (void)
			 {
			 }

	int		 main (void);
	
			 // ===============================================
			 // Sendresult event handler
			 // ===============================================
	void	 onsendresult (int code)
			 {
			 	// Rollback all changes if there 
			 	// was an authd error
			 	if ((code == moderr::err_authdaemon))
			 		authd.rollback ();
			 }
	
	
protected:

	appconfig		 conf;			///< Modules configuration data

	
			 //	 =============================================
			 /// Configuration handler 
			 //	 =============================================
	bool     confSystem (config::action act, keypath &path, 
					  	 const value &nval, const value &oval);	
	
			 //	=============================================
			 /// validate the given configuration
			 /// \return true on ok / false on failure
			 //	=============================================
	bool	 checkconfig (value &v);	
	
			 //	=============================================
			 /// Read the current loaded network module
			 /// and sends the network configuration to the 
			 /// standard output or gives an error.
			 /// \return true on ok / false on failure
			 //	=============================================
	bool	 readconfiguration (void);


			 //	=============================================
			 /// Delete configuration
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 deleteconfiguration (const value &v);
	
	
			 //	=============================================
			 /// Install configuration using authd
			 /// \param v postdata
			 /// \return true on ok / false on failure
			 //	=============================================			 			 
	bool	 installconfig (const value &v);
	
	
			 //	=============================================
			 /// Reload services using authd
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 reloadservices (void);
	
	
			 //	=============================================
			 /// Commit amavis configuration
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 commitconfig (void);

	
			 //	=============================================
			 /// Revert amavis configuration, this cannot
			 /// be done after a configuration has already
			 /// been committed
			 /// \return true on ok / false on failure
			 //	=============================================			 
	bool	 revertconfig (void);

	
			 //	=============================================
			 /// Writes amavis configuration
			 /// \param v given post data
			 /// \return true on ok / false on failure
			 //	=============================================
	bool 	 writeconfiguration (const value &v);
				
};

#endif
