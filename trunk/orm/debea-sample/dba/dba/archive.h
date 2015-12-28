// File: archive.h
// Purpose: Base class for archives
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2002
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAARCHIVE_H
#define DBAARCHIVE_H

#include "dba/defs.h"
#include "dba/convspec.h"

#ifdef DBA_COMPAT_1_0
  #include "dba/ostream.h"
  #include "dba/istream.h"
#endif

/**
  Database and Archive core library
*/
namespace dba {

class IStream;
class OStream;

/**
  Archive class represents database that has relational or object-oriented structure and can contain
  object of classes derived from storeable class.

  To store object in database you must do following steps:
  -# derive from Storeable class
  -# define store table for object
  -# get stream from archive
  -# bind additional variables to stream (optional)
  -# open stream
  -# get or put objects into stream.

  There are two types of streams. Output stream is for storing objects into database and Input stream is for
  getting stream from database.

  You can bind C++ variable to database column and when retrieving query results values that are
  binded will autmaticaly updated. Binded variables can be created from input stream or output stream
  
  If you bind variable to input stream then every time you get an object from database binded
  variable will be updated. If you bind variable to output stream then every time object
  is inserted into database values from binded variable are inserted into database too.

  There is one limitation when using binded variables and output streams - alhtoungh you can
  bind to any table and any column in database, but only columns from tables that are currently
  updated by storing object would be updated. In other words, when 
  %SQL update is constructed to store particular object this query will be extended to update
  binded variables _if_ they are binded to table that is used by object being updated.

  Streams represents connections to database. This means that if get two streams from Archive
  you open two connections to database. Connections are reusable and currently they are
  never closed by archive. Call to Archive::open opens initial connection. Every next connection
  that is internally open by archive is open with the same connect string that initial connection.
  @ingroup api
*/
class dbaDLLEXPORT Archive {
  public:
    /**
      Constructor
    */
    Archive() {};
    /**
      Opens archive 
      @param pOpenStr connect string passed to internal Database object
    */
    virtual void open(const char* pOpenStr) = 0;
    /**
      Check if archive is open
      @returns true if archive is open, false otherwise
    */
    virtual bool isOpen() const = 0;
    /**
      Destructor
    */
    virtual ~Archive() {};
    
    /**
      Create input stream. Returned value must be deleted
      @return IStream
    */
    virtual IStream* getInputStream() = 0;
    /**
      Create output stream. Returned value must be deleted
      @return IStream
    */
    virtual OStream* getOutputStream() = 0;
};

};//namespace

#endif


