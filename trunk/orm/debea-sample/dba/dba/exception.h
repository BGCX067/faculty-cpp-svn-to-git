// File: exception.h
// Purpose: Exception classes
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAEXCEPTION_H
#define DBAEXCEPTION_H

#include <string>
#include "dba/defs.h"

namespace dba {

/**
Base exception class for dba exceptions. This class is intended for 
derivation only.
@ingroup api
*/
class dbaDLLEXPORT Exception : public std::exception {
  public:
    /**
      Get exception message
    */
    virtual const char* what() const throw() { return mCause.c_str(); }
    /**
      Destructor
    */
    virtual ~Exception() throw() {};
  protected:
    /**
      Constructor
      @param pText error text
    */
    Exception(const std::string& pText);
    /**
      Constructor
      @param pText error text
    */
    Exception(const char* pText);
  private:
    std::string mCause;
};

/**
  Database exception is thrown if other that query language related error occurs when using database classes.
  Usually catching this error means that there is a problem with connection to database or data file.
  Bad queries and commands send to database are indcated by throwing SQLException
  @ingroup api
*/
class dbaDLLEXPORT DatabaseException : public Exception {
  public:
    /**
      Constructor
      @param pText error text
    */
    DatabaseException(const std::string& pText) : Exception(pText.c_str()) {};
};

/**
  Database exception related to plugin operations
*/
class dbaDLLEXPORT DbPluginException : public DatabaseException {
  public:
    /**
      Constructor
      @param pText error text
    */
    DbPluginException(const std::string& pText) : DatabaseException(pText.c_str()) {};
};

/**

  Base class for exceptions thrown when daba from database is manipulated,
  for example conversion from string to int failed, bad sql query was executed 
  or parser could not parse corrupted csv file.
  @ingroup api
*/
class dbaDLLEXPORT DataException : public Exception {
  public:
    /**
      Constructor
      @param pText error text
    */
    DataException(const std::string& pText) : Exception(pText.c_str()) {};
};

/**
  Thrown when API is misused.
  @ingroup api
*/
class dbaDLLEXPORT APIException : public Exception {
  public:
    /**
      Constructor.
      @param pText exception description
    */
    APIException(const std::string& pText) : Exception(pText.c_str()) {};
};

/**
  SQLException is thrown if query or command send to database has invalid syntax or there was other error
  from backend server when processing query.
  @ingroup api
*/
class dbaDLLEXPORT SQLException : public DataException {
  public:
    /**
      Constructor
      @param pCause error text
    */
    SQLException(const char* pCause) 
      : DataException(pCause)
    {};
    /**
      Set query that caused exception
      @param pQuery query value
    */
    void setQuery(const char* pQuery) { mQuery = pQuery; };
    /**
      Get query that caused exception
    */
    const char* getQuery() const { return mQuery.c_str(); };
    /**
      Destructor
    */
    virtual ~SQLException() throw() {};
  private:
    std::string mQuery;
};

/**  
  Exception thrown by ConnectStringParser
*/
class dbaDLLEXPORT ConnectStringParserException : public Exception {
  public:    
    /**
      Constructor
    */
    ConnectStringParserException(const char* pWhat) : Exception(pWhat) {};
};

/**
Thrown when suitable data conversion filter cannot be found for given data type

  @author Lukasz Michalski <lm@zork.pl>
*/
class dbaDLLEXPORT FilterNotFoundException : public DataException {
  public:
    FilterNotFoundException(const std::string& pText) : DataException(pText) {};
};

} //namespace

#endif
