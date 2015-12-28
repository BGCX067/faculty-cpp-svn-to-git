// File: sqlarchive.h
// Purpose: Archive for SQL databases
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASQLARCHIVE_H
#define DBASQLARCHIVE_H

#include "dba/archive.h"
#include "dba/sqlistream.h"
#include "dba/sqlostream.h"
#include "dba/filtermapper.h"

namespace dba {

class DbPlugin;

/**
  Archive of objects stored in %SQL database
  @ingroup api
*/
class dbaDLLEXPORT SQLArchive : public Archive {
  //FIXME allow NULL table names in binded vars - this should store binded var in the youngest child table
  //FIXME add API for calling stored procedures
  //FIXME improve connection management by closing too many open connections.
  public:
    /**
      default error handler for archive. It throws SQLException or DatabaseException depends on error type.
      @param pOwner not used
      @param pType type of error
      @param pMsg error description from Database plugin
      @sa errorHandlerFunc
    */
    static bool handleError(void* pOwner, int pType, const char* pMsg);
    /**
      Constructor.
      Archive does not take ownership on passed Database pointer, you have
      to delete it using DbPlugin instance that was used to load it.
      @see open(const char* pPluginName, const char* pConnectString) for better alternative
      @param pDb sql database driver for archive
    */
    SQLArchive(Database* pDb = NULL);
    /**
      Set database plugin for archive. Must be called on closed archive.
      Archive does not take ownership on this pointer, you have
      to delete it using DbPlugin instance that was used to load it.
      @see open(const char* pPluginName, const char* pConnectString) for better alternative
      @param pDb database loaded using DbPlugin
    */
    void setDatabase(Database* pDb);
    /**
      Get pointer to database that archive is using.
    */
    Database* getDatabase();
    /**
      Set fetcher on archive.
      @param pFetcher new id fetcher
    */
    void setIdFetcher(SQLIdFetcher* pFetcher);
    /**
      Get associated id fetcher or NULL if none was set
    */
    SQLIdFetcher* getIdFetcher() const;
    /**
      Connect to already initialized database. You have to set database first to use this function, or (better)
      use open(pPluginName, pConnectString), which will load plugin and create database 
      automatically.
      @param pConnectString ConnectString as string
      @see open(pPluginName, pConnectString) for better alternative
    */
    virtual void open(const char* pConnectString);
    /**
      Load database plugin, check its version, initialize it and 
      connect to database using pConnectString argument
      @param pPluginName name of plugin without extension
      @param pConnectString plugin-specific database connection description
      @see @ref conn_str_driver_notes for connect string description
    */
    void open(const char* pPluginName, const char* pConnectString);
    /**
      Initialize internal plugin and create database instance owned by SQLArchive.
      Used by open(const char* pPluginName, const char* pConnectString)
      @param pPluginName name of plugin library to load
    */
    void initPlugin(const char* pPluginName);
    /**
      Check if archive is open
      @returns true if archive is open or false otherwise.
    */
    virtual bool isOpen() const;

#ifdef DBA_COMPAT_1_0    
    SQLIStream* getOIStream();
    SQLOStream* getOOStream();
#endif    
    
    virtual IStream* getInputStream();
    virtual OStream* getOutputStream();
    
    /**
      Create SQLIStream using one connection from pool. If you want to load and store
      objects using single connection then consider using Transaction and create
      streams from there.
      @returns Object Input Stream 
    */
    SQLIStream getIStream();
    /**
      Create output stream using one connection from pool.
      @returns Object Output Stream 
    */
    SQLOStream getOStream();
    /**
      Get connection from archive. If all connections are used then new connection will be created using ConnectString passed to open().
      @warning returned connection is removed from connection pool managed by SQLArchive - you need to close and delete it yourself.
      @returns DbConnection instance
    */
    DbConnection* getConnection();
    /**
      Get number of currently used connections
    */
    int getUsedConnections() const;
    /**
      Get number of connections that are available for streams and getConnection() function
    */
    int getAvailableConnections() const;
    /**
      Get list of available tables. Must be called after open()
      @return list of names of available %SQL tables
    */
    std::list<std::string> getTableNames();
    /**
      Set new conversion specification for archive. Call to this method will update
      conversion specification on internal database object and all 
      connections to database that are managed by this archive
      @param pSpecs new conversion specification
    */
    virtual void setConversionSpecs(const ConvSpec& pSpecs);
    /**
      Get current conversion specification from internal database object
    */
    virtual const ConvSpec& getConversionSpecs() const;
    /**
      Change default filter for given C++ type. Used when sending SQL queries using streams.
      New mapping for the same C++ type will replace existing one.
      @param pType C++ type returned by typeid()
      @param pFilter New default filter to use
    */
    void mapType(const std::type_info& pType, StoreableFilterBase* pFilter);
    /**
      Check if this instance owns mDb
    */
    bool isDatabaseOwner() const { return mPlugin != NULL; }
    /**
      Destructor
    */
    virtual ~SQLArchive();
  protected:
    /**
      Get or allocate new connection
      @param pRelease if true then remove connection from connection pool
    */
    DbConnection* getFreeConnection(bool pRelease = false);
    /**
      Pointer to internal database object 
    */
    Database* mDb;
    /**
      Plugin loaded for database object creation - NULL if database is set 
      using constructor or setDatabase function. NULL value also means that we 
      are not owner of mDb pointer
    */
    DbPlugin* mPlugin;
    /**
      List of open connections 
    */
    std::list<DbConnection*> mConnections;
    /**
      Connect string used to create new connection
    */
    std::string mConnectStr;
    /**
      SQLFetcher. Must not be NULL when streams are created.
    */
    SQLIdFetcher* mFetcher;
    /**
      Mappings for creating %SQL queries from SQL object in streams
    */
    FilterMapper mFilterMapper;
    //!close all connnections and delete mPlugin and mDb (if owned, see mPlugin description)
    void destroyPlugin();
    //!close all connections to database
    void closeAllConnections();
  private:
};

#ifdef DBA_COMPAT_1_0    
typedef SQLIStream SQLOIStream;
typedef SQLOStream SQLOOStream;
#endif


};//namespace


#endif
