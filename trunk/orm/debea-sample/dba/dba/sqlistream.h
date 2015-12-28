// File: sqlistream.h
// Purpose: Object input stream for SQL databases
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASQLISTREAM_H
#define DBASQLISTREAM_H

#include "dba/istream.h"
#include "dba/sql.h"
#include "dba/shared_ptr.h"

namespace dba {

/**
  IStream implementation for %SQL based archives
  @ingroup api
*/
class dbaDLLEXPORT SQLIStream : public IStream {
    friend class SQLArchive;
    friend class Transaction;
  public:
    /**
      Copy constructor increases internal DbConnection usage
    */
    SQLIStream(const SQLIStream& pStream);
    /**
      Assigment opearator increases internal DbConnection usage.
    */
    SQLIStream& operator=(const SQLIStream&);
    /**
      Get %SQL query string created by stream after open()
    */
    const std::string& getQuery() const;
    virtual bool getNext(Storeable* pObject);
    /**
      Open stream for Storeable object
      @param pObject object type for stream preparation
      @param pMainTable custom name of pObject root store table 
    */
    virtual void open(Storeable& pObject, const char* pMainTable = NULL);
    /**
      Open stream using %SQL SELECT query instead of store tables. Returned columns should
      match all store table fields from all store tables for pObject and binded variables. 
      If there is no field for store table entry then member
      will not be modified when data is retrieved.
      @param pObject object type for stream preparation
      @param pQuery SQL query.
      @param pMainTable custom name of object root store table
    */
    void openFromQuery(Storeable& pObject, const SQL& pQuery, const char* pMainTable = NULL);
    /**
      Update binded vars but don't fill object passed to open()
      Must be called after open() and this method does not advance to next row in result
    */
    virtual bool updateVars();
    virtual void close();
    virtual void destroy();
    /**
      Set additional %SQL WHERE to query that is sent to database. Must be called before open()
      @param pQueryPart where part of %SQL query (without WHERE word)
    */
    virtual void setWherePart(const SQL& pQueryPart);
    /**
      Set WHERE to load only one object with given id.
      Must be called before open or openFromQuery
      @param pId id of object to load
    */
    virtual void setWhereId(int pId);
    /**
      Send SQL query to database and get results.
      Must be called before open or openFromQuery
      @param pQuery query to send.
    */
    DbResult* sendQuery(const SQL& pQuery) const;
    /**
      Get conversion specification used by this stream.
      @return conversion specifications
    */
    const ConvSpec& getConversionSpecs() const;
    virtual ~SQLIStream();
  private:
    /**
      Proxy class that allows SQL to fill its into() variables when calling
      fetchRow() and to use driver DbResult interface.
    */
    class SQLDbResult : public DbResult {
      public:
        SQLDbResult(DbResult* pRes, const FilterMapper& pMapper, const SQL& pSQL);
        virtual int columns() const { return mResult->columns(); }
        virtual const DbColumn& getColumn(int pIndex) const { return mResult->getColumn(pIndex); }
        virtual int getColumnIndex(const char* pColumnName) const { return mResult->getColumnIndex(pColumnName); }
        virtual bool fetchRow();
        virtual ~SQLDbResult() { delete mResult; }
      protected:
        virtual long doGetInt(int pField) const { return mResult->getInt(pField); }
        virtual const char* doGetString(int pField) const { return mResult->getString(pField); }
        virtual double doGetDouble(int pField) const { return mResult->getDouble(pField); }
        virtual struct tm doGetDate(int pField) const { return mResult->getDate(pField); }
        virtual bool doCheckNull(int pField) const { return mResult->isNull(pField); }
      private:
        DbResult* mResult;
        const FilterMapper& mFilterMapper;
        SQL mQuery;
    }; 

    typedef enum {
      WHERE_NOT_SET,
      WHERE_CUSTOM,
      WHERE_ID
    } whereType;
    SQLIStream(DbConnection* pConn, FilterMapper* pMapper);
    void setWhere(const SQL& pWherePart);
    void createSelect();
    std::string createVarSelectFields() const;
    void doQuery();
    void fillBindedVars();
    void applyFilter(DbResult& pRes, StoreableFilterBase& pFilter, const char* pFieldName, Database::StoreType pFieldType);
    virtual void setIdsCondition(const char* pFKeyName, id pRelationId, const std::vector<id>& pIds);

    shared_ptr<DbResult> mResult;
    DbConnection* mConn;
    FilterMapper* mFilterMapper;
    std::string mWherePart;
    std::string mFromPart;
    std::string mQuery;
    bool mRowFetched;
    whereType mWhereSet;
};

};//namespace


#endif
