// File: defs.h
// Purpose: Missing core functions and utilites
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBADEFS_H
#define DBADEFS_H

#define DBA_API_VER 1
#define HAVE_DLOPEN

#if defined(_MSC_VER)
  // disable warnings
  #pragma warning (disable : 4290)  //throw specification
  // testlib and all static libs that use libdba.dll
  #if defined(DEBEA_USINGDLL)
    #define dbaDLLEXPORT __declspec(dllimport)
  #else
    #if defined(DLL_EXPORTS)
      //compiling libdba.dll 
      #define dbaDLLEXPORT __declspec(dllexport)
    #else
      //compiling libdba.lib
      #define dbaDLLEXPORT
    #endif
  #endif
#else
  #define dbaDLLEXPORT
#endif

#if !defined(_WIN32)
  #define HAVE_LOCALTIME_R 1
#endif

#include <time.h>

namespace dba {

/**
  localtime_r for unix, localtime for msw
*/
dbaDLLEXPORT tm* 
dba_localtime(const time_t* pFrom, struct tm* pTo);

/** @defgroup store_table Store table macros*/
/** @defgroup api Public API*/
/** @defgroup filters Conversion filters for store tables*/

/**
  Type for object id
  @ingroup api
*/
typedef unsigned int id;

#ifdef DBA_COMPAT_1_0
typedef id stid;
#endif

}; //namespace

#endif
