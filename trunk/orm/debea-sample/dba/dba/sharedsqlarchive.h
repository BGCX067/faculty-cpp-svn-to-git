// File: sharedsqlarchive.h
// Purpose: SQLArchive with connection management and transaction support
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2003
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASHAREDSQLARCHIVE_H
#define DBASHAREDSQLARCHIVE_H

#include "dba/sqlarchive.h"
#include "dba/shared_ptr.h"

namespace dba {

class SharedSQLArchive;

/**
  Transaction can be created from archive to enclose set of database operations in single reversable unit.
  To reverse all operations call rollback() on transaction.
  
  Transactions can be created in two modes. If TRANS_USE_LAST (default) mode is used, then 
  first call to SharedSQLArchive::createTransaction will get DbConnection from connection
  pool and create transaction on it. All subsequent calls to SharedSQLArchive::createTransaction
  will create child Transactions using already selected connection. This way you can enclose
  parts of code to be in single transaction context like this:
  
@code
void 
myStoreFunction() {
  Trasaction local(mArchive.createTransaction());
  //do some queries here
}

void myDataProcessing() {
  Trasaction tp(mArchive.createTransaction());
  myStoreFunction();
  myStoreFunction();
  //some other queries
}
@endcode

When myDataProcessing is called then database transaction 
is started when tp is created and commited when tp is destructed.
Creation of "local" Transaction instance do nothing, because it is
called in already created transaction context.

If myStoreFunction sould be called where there was no active transaction object 
created, then SharedSQLArchive will assign DbConnection to it and start new transaction.

Second mode, TRANS_NEW will break exisiting transaction context and create new one. That is, 
if in above example "local" would be created using TRANS_NEW mode then new DbConnection would 
be assigned to it and new transaction would be started on that connection.

All streams returned by Transaction object shares one internal DbConnection and all operations 
on that streams are done in transaction context. You should not close or destroy returned streams
Transaction destructor will take care of it.

The default action when Transaction is destroyed is to commit transaction to database. If you want to 
rollback all operations instead, then you should call rollback() before top-most Transaction instance
is destructed. Calling rollback will mark Transaction to be rolled back - %SQL ROLLBACK TRANSACTION query
is sent when last Transaction instance in current transaction context goes out of scope.

@ingroup api
@sa SharedSQLArchive::createTransaction
*/
class dbaDLLEXPORT Transaction {
    friend class SharedSQLArchive;
  public:
    /**
      Type of transaction to create
    */
    typedef enum {
      TRANS_USE_LAST, //!<create child of last existing transaction
      TRANS_NEW //!<create new transaction
    } creationType;
    /**
      Copy constructor
      @param pTransaction object to copy from
    */
    Transaction(const Transaction& pTransaction);
    /**
      Assigment operator
      @param pTransaction object to copy from
    */
    Transaction& operator=(const Transaction& pTransaction);
#ifdef DBA_COMPAT_1_0    
    shared_ptr<SQLIStream> getOIStream();
    shared_ptr<SQLOStream> getOOStream();
#endif    
    /**
      Get SQLOStream related to Transaction
      @warning this stream should not be closed - To close streams delete Transaction instance
      @returns SQLOStream instance 
    */
    SQLOStream getOStream();
    /**
      Get SQLOStream related to Transaction
      @returns SQLOStream instance 
    */
    SQLIStream getIStream();
    /**
      Rollback transaction. All changes done from time when first 
      topmost parent Transaction instance was created will be rolled back when 
      last instance of child Transaction will be destructed.
    */
    void rollback();
    /**
      Commit transaction. Commits all current changes to streams and starts new transaction just after it.
    */
    void commit();
    /**
      Check if transaction is rolled back
    */
    bool isRolledback() const;
    /**
      Destructor
    */
    ~Transaction() throw();
  private:
    Transaction(DbConnection* pConn, SQLIdFetcher* pFetcher, FilterMapper* pMapper, const shared_ptr<bool>& pRollbackFlag);

    DbConnection* mConn;
    SQLIdFetcher* mFetcher;
    FilterMapper* mFilterMapper;
    shared_ptr<bool> mRollbackFlag;
    
    void cleanStreams();
};

/**
SQLArchive that supports transactions on object level
@ingroup api
*/
class dbaDLLEXPORT SharedSQLArchive : public SQLArchive {
    friend class Transaction;
  public:
    /**
      Constructor.
      @see SQLArchive constructor for explanations
      @param pDatabase Database object describing %SQL database.
    */
    SharedSQLArchive(Database* pDatabase = NULL);
    /**
      Creates Transaction object. When this object will be deleted all changes to database will be commited if
      there was no error or rolled back if sql error occurs on streams that belong to Transaction. For one
      instance of SQLArchive, first created Transaction object will open %SQL transaction on single database
      connection. Last destructed transaction object will commit or rollback %SQL transaction.
      @param pType of transaction. See Transaction::creationType for explanations
    */
    Transaction createTransaction(Transaction::creationType pType = Transaction::TRANS_USE_LAST);
    /**
      Destructor.
    */
    virtual ~SharedSQLArchive() throw();
  private:
    DbConnection* mLastTransConnection;
    shared_ptr<bool> mRollbackFlag;
};

};//namespace



#endif
