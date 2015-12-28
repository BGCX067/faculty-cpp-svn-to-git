// File: sqlostream.h
// Purpose: Object output stream for SQL databases
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASQLOSTREAM_H
#define DBASQLOSTREAM_H

#include "dba/ostream.h"
#include "dba/sql.h"

namespace dba {

class SQLIdFetcher;

/**
  OStream implementation for %SQL based archives
  @ingroup api
*/
class dbaDLLEXPORT SQLOStream : public OStream {
    friend class SQLArchive;
    friend class Transaction;
  public:
    /**
      Copy constructor increases internal DbConnection usage
    */
    SQLOStream(const SQLOStream& pStream);
    /**
      Assigment opearator increases internal DbConnection usage.
    */
    SQLOStream& operator=(const SQLOStream&);
    /**
      Open stream and prepare it to object store
      @param pMainTable custom root table name for object. If 
    */
    virtual void open(const char* pMainTable = NULL);
    virtual void begin();
    virtual void commit();
    virtual void rollback();
    virtual bool isOpen() const;
    virtual void close();
    virtual void destroy();
    /**
      Send SQL update query to database and get results
      @param pQuery query to send.
    */
    int sendUpdate(const SQL& pQuery);
    /**
      Get conversion specification used by this stream.
      @return conversion specifications
    */
    const ConvSpec& getConversionSpecs() const;
    virtual ~SQLOStream();
  protected:
    //!used connection
    DbConnection* mConn;
    //!object responsible for assigning id for newly created Storeable
    SQLIdFetcher* mFetcher;
    //!database that maps data type to StoreableFilterBase class for conversion
    FilterMapper* mFilterMapper;
  private:
    virtual void assignId(Storeable* pObject) throw (Exception);
    SQLOStream(DbConnection* pConn, SQLIdFetcher* pFetcher, FilterMapper* pMapper);
    std::string createInsert(int id,const Storeable& pObject,mt_class* pTable);
    std::string createUpdate(const Storeable& pObject,mt_class* pTable);
    std::string applyFilter(StoreableFilterBase& pFilter, Database::StoreType pType);
    
    virtual std::vector<id> loadRefData(const char* pTable, const char* pFkName, id pCollId, id pId);
    virtual bool deleteRefData(const std::vector<id>& pIds, const char* pTableName);
    virtual bool isCollectionFilterSupported() const { return true; };
    virtual bool erase(Storeable* pObject);
    virtual bool update(Storeable* pObject);
    virtual bool store(Storeable* pObject);
    
    std::string mCurrentTable;
};

};//namespace


#endif
