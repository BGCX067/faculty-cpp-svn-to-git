//
// C++ Interface: fileutils
//
// Description: 
//
//
// Author: Lukasz Michalski <lm@zork.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DBAFILEUTILS_H
#define DBAFILEUTILS_H

#include "dba/exception.h"

namespace dba {

/**
  Exception for libxml2 related errors
*/
class dbaDLLEXPORT FileException : public DatabaseException {
  public:
    /**
      Constructor
      @param pError exception description
    */
    FileException(const char* pError) : DatabaseException(pError) {};
};

/**
  File utilities
*/
class dbaDLLEXPORT FileUtils {
  public:
    static bool exists(const char* pFilename);
};

} //namespace

#endif
