// File: connectstring.h
// Purpose: SQL database connect string
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2004
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBACONNECTSTRING_H
#define DBACONNECTSTRING_H

#include "dba/defs.h"
#include <string>
#include <map>

namespace dba {

/**
Class for parsing parameters from connect string.
Connect string contains name=value pairs that describes how to connect to database.

Standard names:
 - user: name of user that connects to database
 - password: password for user that connects to database
 - dbname: database, dsn or database file name
 - host: IP or host name where to connect to
 - port: TCP port where to connect to
 - odbcparams: ODBC connection string
 
Spaces in names and values can be escaped using %SQL syntax, for example:

@code
dbname='C:\\Program Files\\myApp\\database.sqt3'
dbname=mydb pass=O''Connor
@endcode

@section conn_str_driver_notes Driver notes

@subsection conn_str_driver_notes_odbc ODBC

ODBC drivers accepts following parameters:

- odbcparams
- dbname
- unicode

Driver searches for dbname first and if found passes all its contents as DSN=\<value\> to SQLConnect. 
If not found, then driver searches for odbcparams and passes its contents to SQLConnect. If odbcparams is not found
then driver passes full connect string to SQLConnect.

You can control how driver should send queries to database using unicode setting. Default behaviour is to try to convert
queries to unicode and call SQLExecDirectW with unicode string. If conversion cannot be performed then query is passed
unmodified to SQLExecDirect. Source encoding of query is determined from DbConnection::mConvSpecs::mDbCharset.

If unicode is set to 0, then driver does not try to convert query to unicode but passes it directly to SQLExecDirect.
If unicode is set to 1, then driver tries to convert query to unicode and if failed then handleError with DBA_DB_ERROR code is 
called (in case when driver is used by SQLArchive this causes DatabaseException to be thrown).

When driver retrieves unicode data from database it performs conversion to encoding specified in DbResult::mConvSpecs::mDbCharset. If charset is not set to dba::ConvSpec::UTF8 and OS does not support requested conversion, then handleError is called with DBA_DB_ERROR code (in case when driver is used by SQLArchive this causes DatabaseException to be thrown).

If driver retrieve non-unicode data from database then it returns it to getString unmodified.

Examples of use:
@code
dbname=mydsn
odbcparams="DSN=mydsn" unicode=0
FILEDSN=myfiledsn
@endcode

@subsection conn_str_driver_notes_pgsql PostgreSQL

PostgresSQL database driver accepts all parameters that libpq supports, because connect string is directly passed to 
driver PGConnect routine. Common parameters that you should use to connect to database:

- dbname
- password
- user
- host
- port

See postgres documentation for meaning of those parameters.

@subsection conn_str_driver_notes_sqlite3 Sqlite3

Sqlite3 driver accepts dbname parameter and treats it as full path to database file name. If "dbname" is not found then
connect string contents is iterpreted as database file name.

@ingroup api
*/
class dbaDLLEXPORT ConnectString {
  public:
    /**
      Default constructor
    */
    ConnectString() {};
    /**
      Constructor.
      @param pStr connect string.
    */
    ConnectString(const char* pStr);
    /**
      Get username
      @returns value of user keyword  
    */
    std::string username() const;
    /**
      Get password
      @returns value of pass keyword  
    */
    std::string password() const;
    /**
      Get name of database
      @returns value of dbname keyword
    */
    std::string dbname() const;
    /**
      Get hostname of rdbms
      @returns value of dbname keyword
    */
    std::string host() const;
    /**
      Get TCP/IP port 
      @returns value of dbname keyword
    */
    std::string port() const;
    /**
      Get value of custom parameter
      @param pParam parameter name
      @returns parameter value or empty string if not found
    */
    std::string getParam(const char* pParam) const;
    /**
      Get number of parsed params 
    */
    int getParamCount() const { return mParams.size(); }
    ~ConnectString() {};
  protected:
    std::map<std::string,std::string> mParams;
};

};

#endif
