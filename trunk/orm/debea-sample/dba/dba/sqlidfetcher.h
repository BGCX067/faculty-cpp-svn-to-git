// File: sqlidfetcher.h
// Purpose: Interface for getting next Storeable::id from dba::Database
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASQLIDFETCHER_H
#define DBASQLIDFETCHER_H

#include "dba/defs.h"

namespace dba {

class DbConnection;

/**
  This class is responsibile for creating new object id when storing Storeable derived object into SQLArchive.
  Implementation should provied getNextId method that returns next avaiable id
  @ingroup api
*/
class dbaDLLEXPORT SQLIdFetcher {
  public:
    virtual ~SQLIdFetcher() {};
    SQLIdFetcher() {};
    /**
      Get next object id from database
      @param pConn database connection
      @param pRootTableName table name
    */
    virtual int getNextId(DbConnection& pConn, const char* pRootTableName) = 0;
};

};//namespace


#endif
