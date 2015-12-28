// File: genericfetcher.h
// Purpose: SQLIdFetcher generic implementation
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBAGENERICFETCHER_H
#define DBAGENERICFETCHER_H

#include "dba/sqlidfetcher.h"
#include "dba/sqlarchive.h"
#include <memory>

namespace dba {

/**
  Generic SQLIdFetcher implementation. Needs table named debea_object_count
  in database with field id.
  @ingroup api
*/
class dbaDLLEXPORT GenericFetcher : public dba::SQLIdFetcher {
  public:
    GenericFetcher();
    virtual int getNextId(dba::DbConnection& pConn, const char*) {
      std::auto_ptr<dba::DbResult> res(pConn.sendQuery("SELECT id FROM debea_object_count"));
      if (!res->fetchRow())
        throw dba::DataException("Unable to allocate next object id");
      int id = res->getInt(0);
      res.reset();
      pConn.sendUpdate("UPDATE debea_object_count SET id = id + 1");
      return id;
    };
    virtual ~GenericFetcher();
};

};//namespace


#endif
