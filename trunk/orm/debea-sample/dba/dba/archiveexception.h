// File: archiveexception.h
// Purpose: exception thrown by archive (depreciated, use DataException derived class)
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAARCHIVEEXCEPTION_H
#define DBAARCHIVEEXCEPTION_H

#include "dba/exception.h"

namespace dba {

#ifdef DBA_COMPAT_1_0

/**
  Exception thrown by Archive derived classes.
*/
class dbaDLLEXPORT ArchiveException : public DataException {
  public:
    /**@internal
      Constructor
    */
    ArchiveException(const std::string& pCause) : dba::DataException(pCause) {};
};

#endif

};//namespace


#endif
