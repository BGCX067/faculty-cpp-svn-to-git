// File: dbupdate.h
// Purpose: Support for loading Database drivers
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBADBUPDATE_H
#define DBADBUPDATE_H

#include <list>
#include <string>
#include "dba/defs.h"

namespace dba {

class SharedSQLArchive;
class DbUpdateScriptsParser;
class SQLOStream;

/**
  DbUpdate use DbUpdateScriptsParser interface to get collection of %SQL statements for 
  database schema update. This interface allows progress bar implementation for 
  derived classes.
  
  This interface requires %SQL table db_version to exists in database with following schema:
@code
CREATE TABLE db_version (
  version VARCHAR(128)
);
@endcode
@ingroup api
*/
class dbaDLLEXPORT DbUpdate {
  public:
    /**
      Constructor
      @param pArchive already open %SQL database where update will be performed
      @param pFileParser parser capable of reading update scripts
    */
    DbUpdate(SharedSQLArchive& pArchive, DbUpdateScriptsParser& pFileParser);

    /** 
      Get version string from database table "db_version", field "version"
    */
    virtual std::string getCurrentVersion() const;

    /**
      Update database. This is done by reading query list by passed DbUpdateScruptParser 
      and calling onSendUpdate for every read query. After update new version string is set in database
      @param pVersion target version for update. If empty, then update will call all available scripts from sets.
    */
    void update(const std::string& pVersion = "");

  protected:
    /** 
      Send single query to database
      @warning all BEGIN (TRANSACTION) and COMMIT statements are removed
      @warning all CREATE DATABASE statements are querying outside transaction
      @param pOStream stream used to send query
      @param pQuery query contents
      @return number of updated records 
      FIXME better statement recognition (mixed case) and empty query (do not send white chars only)
    */
    virtual int onSendUpdate(SQLOStream& pOStream, std::string& pQuery);

    /**
      Parser used to get queries from scripts
    */
    DbUpdateScriptsParser& mFileParser;


  private:
    /**
      Update database version
      @param pNewestScript name of last 
    */
    void updateVersion(const std::string& pCurrentVersion);
    /**
      Set version string in database table "db_version", field "version"
      @param pVersion new version string to set
    */
    virtual void setCurrentVersion(const std::string& pVersion) const;
    /**
      remove all CREATE DATABASE statements in the query scripts from transaction by
      surrounding them with 'commit' and 'begin'
    */
    inline void makeCreateDatabaseQueriesOutsideTransaction(std::string& pQuery, bool pUpperCase) const;
    /**
      remove all BEGIN (TRANSACTION) and COMMMIT statements in the query scripts
      Also clear the query if it consists of whitechars or ';' only
    */
    inline void delTransactionStatementsFromQuery(std::string& pQuery, const std::string& pSub, size_t pSize) const;
    SharedSQLArchive& mArchive;
}; // class

}; //namespace

#endif
