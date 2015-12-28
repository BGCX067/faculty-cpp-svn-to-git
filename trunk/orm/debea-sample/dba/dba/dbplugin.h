// File: dbplugin.h
// Purpose: Support for loading Database drivers
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAPLUGINLOADER_H
#define DBAPLUGINLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "dba/plugininfo.h"
#include "dba/database.h"
#include "dba/exception.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace dba {

/**
Database plugin loader. 
Loads plugin library and unloads it upon destruction.
Usage:

@code
try {
  DbPlugin* plugin = new DbPlugin("libsqlite3");
  Database* db(plugin->createHandle());
  DbConnection* conn = db->getConnection();
  [...]

  delete conn;
  delete db;
} catch (const dba::Exception& ex) {
  //error handling code
};

//cleanup only when all created databases and connections are deleted
delete plugin;
@endcode
@ingroup api
*/
class dbaDLLEXPORT DbPlugin {
  public:
    /**
      Default constructor creates empty plugin (with no library loaded)
    */
    DbPlugin();
    /**
      Loads library from path
      @param pFilename OS specific path to library name
    */
    DbPlugin(const char* pFilename) throw (Exception);
    /**
      Loads library from path
      @param pFilename OS specific path to library name
    */
    void load(const char* pFilename) throw (Exception);
    /**
      Unloads library
    */
    void close();
    /**
      Creates new Database object
      @return pointer to created Database object
    */
    Database* createHandle() const throw (Exception);
    /**
      Destroys Database object
    */
    void destroyHandle(Database* pDb);
    /**
      Loads plugin information
      @return pluginInfo structure that contains version, vendor and license information
    */
    dbaPluginInfo getInfo() const throw (Exception);
    /**
      Destructor. Calls close() if library is still loaded
    */
    virtual ~DbPlugin();
  private:
    #ifdef _MSC_VER
    HINSTANCE mLibHandle;
    #else
    void* mLibHandle;
    #endif
};

}

#endif
