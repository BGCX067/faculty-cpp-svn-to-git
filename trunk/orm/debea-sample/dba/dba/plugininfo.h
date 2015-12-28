// File: ostream.cpp
// Purpose: Database plugin information structure
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAPLUGININFO_H
#define DBAPLUGININFO_H

#include "dba/defs.h"

#define DBA_VERSION_MAJOR   1
#define DBA_VERSION_MINOR   4
#define DBA_VERSION_RELEASE 0
#define DBA_URL             "http://debea.net"
#define DBA_VENDOR          "Debea"
#define DBA_LICENSE         "wxWindows 3.1"

namespace dba {

/**
  Type of plugin
  @ingroup api
*/
typedef enum {
  DBA_DATABASE_DRIVER = 1, //!<plugin that implements Database interface
  DBA_ARCHIVE_SUPPORT = 2 //!<plugin that implements Archive interface
} pluginType;

/**
Struct describing driver plugin information. 
This struct should be initialized in every database driver for dyamic loading
@ingroup api
*/
extern "C" {
  struct dbaPluginInfo {
    /**
      Name of plugin
    */
    char mName[40];
    /**
      Type of license (Commercial, GPL, public domain, shareware etc)
    */
    char mLicense[40];
    /**
      Plugin description
    */
    char mDescription[1000];
    /**
      Creator of plugin
    */
    char mVendor[40];
    /**
      Web address to plugin homepage
    */
    char mUrl[80];
    /**
      Major part of version of library for which this plugin was build 
    */
    int mVersionMajor;
    /**
      Minor part of version of library for which this plugin was build 
    */
    int mVersionMinor;
    /**
      Release part of version of library for which this plugin was build 
    */
    int mVersionRelease;
    /**
      Type of plugin. See pluginType for available types
    */
    char mPluginType;
  };
};

};

#endif
