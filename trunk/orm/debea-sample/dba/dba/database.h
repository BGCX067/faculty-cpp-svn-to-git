// File: database.h
// Purpose: Base class for SQL driver plugins
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2001
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBADATABASE_H
#define DBADATABASE_H

#include <string>
#include <set>
#include <list>
#include "dba/defs.h"
#include "dba/convspec.h"

namespace dba {

/**
  Abstract base class that represents database in the system. 
  */


class DbResult;
class DbField;
class DbRow;
class DbColumn;
class DbTable;
class DbConnection;

extern "C" {
  struct dbaPluginInfo;
};

/**
  Type of error
  @ingroup api
*/
typedef enum {
  DBA_DB_ERROR, //!<error related to database engine
  DBA_SQL_ERROR //!<error related to queries and commands
} errorTypes;


/**
  Callback function called when error occurs. See ErrorHandler documentation for futher explanations
  @param pObj pointer passed to DbBase::setErrorHandler
  @param pErrorType error type (oe of value from errorTypes enum)
  @param pErrorMsg error message from underlaying driver
  @returns true if error was processed, false otherwise
  @relates errorTypes
*/
typedef bool (*errorHandlerFunc)(void* pObj, int pErrorType, const char* pErrorMsg);

/**
  This is base object for Database, DbConnection and DbResult classes. It contains error
  handling routines and inherits conversion specification for converting data between database
  and application.
  
  Database plugins can create multiple DbConnections and those connections can create multiple DbResult objects.
  When there is an error during data processing and current object cannot handle that error it should propagated up in this tree structure. 
  This class contains pointers that forms tree allowing errors to be propagated from DbResult to DbConnection and up to Database objects.
  Error is propagated only if current DbBase object don't have error handling callback or calling this callback
  returned false. In both cases parent DbBase object is tried. If there is no parent handler and callback function is NULL then error message is printer to stderr.
  
  Conversion specification data should copied when child object is created. It is plugin responsibility to
  copy this data from Database derived class to DbConnection when DbConnection is created and from DbConnection
  to DbResult.
  
  @ingroup api
*/
class dbaDLLEXPORT DbBase : public ConvSpecContainer {
  public:
    /**
      Constructor.
    */
    DbBase();
    /**
      Sets parent error handler. Parent error handler will be tried if this error handler cannot handle error (callback function is NULL or returns false)
      @param pParent parent error handler
    */
    void setParentErrorHandler(DbBase* pParent);
    /**
      Sets new error handler
      @param pOwner this pointer will be passed to errorHandlerFunc as pObj
      @param pFunc user suppiled error handling routine
    */
    void setErrorHandler(void* pOwner, errorHandlerFunc pFunc);
    /**
      Handles error from database backend. Default implementation prints errors on stderr
      @param pErrorType type of error
      @param pMsg error message from database backend
      
    */
    void handleError(int pErrorType, const char* pMsg) const;
    /**
      Get parent error handler or NULL if not set
    */
    DbBase* getParentErrorHandler() const { return mParent; }
    /**
      Destructor
    */
    ~DbBase();
  protected:
    /**
      pointer to parent error handler
    */
    DbBase* mParent;
    /**
      list of child error handlers
    */
    std::set<DbBase*> mChildren;
    /**
      Custom error handling function. Called instead of default handleError function
    */
    errorHandlerFunc mErrorHandler;
    /**
      pointer to data that is passed to custom error handing function
    */
    void* mObj;
};

/**
  Interface for database engines that use relational data model - like %SQL RDBMS
  Plugins for SQLArchive implements this interface.
  @ingroup api
*/  
class dbaDLLEXPORT Database : public DbBase {
  public:
    /**
      Type hints for StoreableFilter instances
    */
    typedef enum {
      UNKNOWN = 0, //!undefined 
      INTEGER, //!<integer values
      FLOAT, //!<float values
      DATE, //!<date and time values
      STRING //!<string values
    } StoreType;
    
    /**major part of version of core library*/
    static const int sVersionMajor;
    /**minor part of version of core library*/
    static const int sVersionMinor;
    /**release part of version of core library*/
    static const int sVersionRelease;
    
    /**
      Create new connection to database
      @param pParams connection parameters
      @return DbConnection object.
      @sa ConnectString for connection parameters description
    */
    virtual DbConnection* getConnection(const char* pParams) = 0;
    /**
      Destructor
    */
    virtual ~Database() {};
  protected:
};

/**
  This class represents connection to database. You can send commands or queries to database
  using this connection object. Most databases does not support multiple query processing on  
  single connection so if you use sendQuery() method for a second time be sure that DbResult from
  first query is destroyed or all rows are fetched.   
  @ingroup api
*/
//FIXME add flag or switch to indicate that use wants to process multiple queries on one connection \
//throw exception if database does not support it.
class dbaDLLEXPORT DbConnection : public DbBase {
  public:
    /**
      Should driver use UNICODE capability 
    */
    typedef enum {
      DEBEA_UNICODE_DEFAULT,
      DEBEA_UNICODE_ON,
      DEBEA_UNICODE_OFF
    } unicode_flag_t;
    /**
      @overload
    */
    DbResult* sendQuery(const std::string& pSql) {
      return sendQuery(pSql.c_str());
    };
    /**
      Send %SQL query to database.
      @param pSql %SQL query to send
      @sa sendUpdate
    */
    DbResult* sendQuery(const char* pSql);
    /**
      Send %SQL commands to database.
      @param pSql %SQL command to send
      @sa sendQuery
    */
    int sendUpdate(const char* pSql);
    /**
      @overload
    */
    int sendUpdate(const std::string& pSql) {
      return sendUpdate(pSql.c_str());
    };
    /**
      Start transaction
    */
    virtual void begin() = 0;
    /**
      Commit transaction
    */
    virtual void commit() = 0;
    /**
      Rollback transaction
    */
    virtual void rollback() = 0;
    /**
      disconnect. After you call this method isValid() will return false.
    */
    virtual void disconnect() = 0;
    /**
      Check if connection is valid. Connection can be invalid because of protocol error or broken
      connection to backend server
      @return true if connection is valid or false otherwise.
    */
    virtual bool isValid() const = 0;
    /**
      Get list of relations that connected database advertise to us
      This list does not contain system tables, only ones that are created by user.
    */
    virtual std::list<std::string> getRelationNames() = 0;
    /**
      Check if connection is used. This flag is useful for classes that will control connection object, ie.
      Database connection managers.
    */
    bool isUsed() const;
    /**
      Increase usage counter. Used by streams.
    */
    void incUsed();
    /**
      Decrease usage counter. Used by streams.
    */
    void decUsed();
    /**
      Destructor
    */
    virtual ~DbConnection() {};
  protected:
    /**@internal
      Default constructor.
    */
    DbConnection();
    /**
      Override with database specific implementation
      @param pSql %SQL query to send
    */
    virtual DbResult* execQuery(const char* pSql) = 0;
    /**
      Override with database specific implementation
      @param pSql %SQL command to send
    */
    virtual int execUpdate(const char* pSql) = 0;
    /**
      Usage counter used by streams.
    */
    int mUseCount;
};


/**
  This class represents result of %SQL query.
  It allows to discover column names and get data from database as one of four C types.
  @ingroup api
*/
class dbaDLLEXPORT DbResult : public DbBase {
  public:
    /**
      Invalid date and time structure returned for NULL values
    */
    static const struct tm sInvalidTm;
    /** @name Dataset operations
    */
    //@{
    /**
      Get number of columns.
      @return number of columns in result.
    */
    virtual int columns() const = 0;
    /**
      Get column description.
      @param pIndex number of column (in query order), starting from zero.
      @return i-th column description.
    */
    virtual const DbColumn& getColumn(int pIndex) const = 0;
    /**
      Get column index by name
      @param pColumnName name of searched column
      @return index of column that has given name
      @remarks if column is not found then driver should call handleError of DBA_DB_ERROR type
    */
    virtual int getColumnIndex(const char* pColumnName) const = 0;
    /**
      Cancel query processing. This operation can be one only once for DbResult and currently is supported for ODBC driver only.
    */
    virtual void cancel() {};
    /**
      Fetches next row from result. If database does not support processing multiple queries
      send on the same connection fetchRow() must return false  before next query can be send on the same
      connection.
      @return true if row was successfully fetched or false otherwise.
    */
    virtual bool fetchRow() = 0;
    //@}
    /** @name Data Access
      Those functions returns data from current row converting internal data representation
      to return data type. Field in row can be specified using column index or field name.
      Column index starts from zero. If %SQL type that driver returned is different from
      returned type, then %SQL type is converted to type to return. If conversion fails, then
      DataException is thrown.
    */
    //@{
    /**
      Get field as string value
      @param pField field name
    */
    const char* getString(const char* pField) const { return getString(getColumnIndex(pField)); }
    /**
      Get field as string value
      @param pField field index
    */
    const char* getString(int pField) const;
    /**
      Get field as int value
      @param pField field name
    */
    long getInt(const char* pField) const { return getInt(getColumnIndex(pField)); }
    /**
      Get field as int value
      @param pField field index
    */
    long getInt(int pField) const;
    /**
      Get field as double value
      @param pField field name
    */
    double getDouble(const char* pField) const { return getDouble(getColumnIndex(pField)); }
    /**
      Get field as double value
      @param pField field index
    */
    double getDouble(int pField) const;
    /**
      Get field as date value
      @param pField field name
    */
    struct tm getDate(const char* pField) const { return getDate(getColumnIndex(pField)); }
    /**
      Get field as date value
      @param pField field index
    */
    struct tm getDate(int pField) const;
    /**
      Check if field contains null value
      @param pField field name
      @return true if field is null, false otherwise
    */
    bool isNull(const char* pField) const { return doCheckNull(getColumnIndex(pField)); }
    /**
      Check if field contains null value
      @param pField field index
      @return true if field is null, false otherwise
    */
    bool isNull(int pField) const { return doCheckNull(pField); }
    //@}
    virtual ~DbResult() {};
  protected:
    /**
      Default constructor
    */
    DbResult() {};
    /**
      Driver specific implementation for getInt
      @param pField index of column
      @return value of field from result
      @see getInt
    */
    virtual long doGetInt(int pField) const = 0;    
    /**
      Driver specific implementation for getString
      @param pField index of column
      @return value of field from result
      @see getString
    */
    virtual const char* doGetString(int pField) const = 0;
    /**
      Driver specific implementation for getDouble
      @param pField index of column
      @return value of field from result
      @see getDouble
    */
    virtual double doGetDouble(int pField) const = 0;
    /**
      Driver specific implementation for getDate
      @param pField index of column
      @return value of field from result
      @see getDate
    */
    virtual struct tm doGetDate(int pField) const = 0;
    /**
      Driver specific implementation for isNull
    */
    virtual bool doCheckNull(int pField) const = 0;
};

/**
  class for column representation in DbResult   
  @ingroup api
*/
class dbaDLLEXPORT DbColumn {
  public:
    /**
      Constructor
      @param pDbCharset database charset in which column name is encoded
    */
    DbColumn(dba::ConvSpec::charset pDbCharset) : mDbCharset(pDbCharset) {};
    /**
      Change encoding of column name
    */
    void setDbCharset(dba::ConvSpec::charset pDbCharset) { mDbCharset = pDbCharset; }
    /**
      get column name
      @returns name of column
    */
    virtual const char* getName() const = 0;
    /**
      Get charset in which column name is encoded
      @return dba charset constant
    */
    ConvSpec::charset getDbCharset() const { return mDbCharset; }
    /**
      Destructor
    */
    virtual ~DbColumn() {};
  protected:
    //! dba charset of column name encoding
    ConvSpec::charset mDbCharset;
};

};//namespace

extern "C" {
  /**
    called by DbPlugin to create Database instance
    @returns Database instance 
  */
  typedef dba::Database* (*dbaDbCreationFunction)();
  /**
    called by DbPlugin to destroy Database instance
  */
  typedef void (*dbaDbDestructionFunction)(dba::Database*);
  /**
    call this function to get information for plugin
    @returns structure that describes information about loaded plugin.
  */
  typedef dba::dbaPluginInfo (*dbaPluginInfoFunction)();
};


#endif
