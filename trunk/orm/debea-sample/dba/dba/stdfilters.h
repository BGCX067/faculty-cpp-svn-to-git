// File: stdfilters.h
// Purpose: Old storeable filters (depreciated)
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDFILTERS_H
#define DBASTDFILTERS_H

#include "dba/defs.h"

#include "dba/string_filter.h"
#include "dba/int_filter.h"
#include "dba/double_filter.h"
#include "dba/datetime_filter.h"
#include "dba/bool_filter.h"

namespace dba {

#ifdef DBA_COMPAT_1_0

/**
Dba filters for std types
*/
class dbaDLLEXPORT stdFilters {
  public:
    //!int filter
    typedef dba::Int Int32;
    //!std::string filter
    typedef dba::String String;
    //!double filter
    typedef dba::Double Double;
    //!struct tm filter
    typedef dba::DateTime DateTime;
    //!bool filter
    typedef dba::Bool Bool;
};

#endif //DBA_COMPAT_1_0

}; //namespace

#endif
