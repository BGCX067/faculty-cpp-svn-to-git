// File: dbupdatescriptparser.h
// Purpose: SQL update scripts parser interface for DbUpdate
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBADBUPDATESCRIPTPARSER_H
#define DBADBUPDATESCRIPTPARSER_H

#include <string>
#include "dba/defs.h"

namespace dba {

/**
  %SQL update scripts parser interface for DbUpdate
  @ingroup api
*/
class dbaDLLEXPORT DbUpdateScriptsParser {
  public:
    /**
      Get next query from script sets
    */
    virtual std::string getNextQuery() = 0;
    /**
      Narrow query set to all queries needed to update database to given version
      @param pVersion version string to which queries should be returned
    */
    virtual void setToVersion(const std::string& pVersion) = 0;
    /**
      Narrow query set to all queries needed to update database from given version
      @param pVersion version string to which queries should be returned
    */
    virtual void setFromVersion(const std::string& pVersion) = 0;
    /**
      Get current version string to which database should be updated
    */
    virtual std::string getToVersion() = 0;
    /**
      Check if there is more queries to send
      @return true if there are more queries to send, false otherwise
    */
    virtual bool isEof() = 0;

    /**
      Called by DbUpdate when scripts list should be prepared for 
      fetching using getNextQuery
    */
    virtual void prepareScriptsList() {};

  protected:
    DbUpdateScriptsParser();

};

}; //namespace

#endif
