// File: stream.h
// Purpose: Base class archive streams
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTREAM_H
#define DBASTREAM_H

#include "dba/membertree.h"
#include "dba/convspec.h"
#include "dba/bindedvar.h"
#include "dba/database.h"
#include "dba/storeable.h"
#include "dba/shared_ptr.h"
#include <list>

namespace dba {

class StoreableFilterBase;

/**
  Stream class is base class for Object Output Stream (OStream) and Object Input Stream (IStream)
  classes
  @ingroup api
  */
class dbaDLLEXPORT Stream {
    friend class BindedBase;
  public:
    /**
      Constructor
    */
    Stream();
    /**
      Bind id value to database column.     
      @param pTable database relation that we want to bind to
      @param pField column that variable will be binded to
      @param pFilter filter that converts database internal representation to int value. 
      @deprecated use bind instead
      @warning this method has no effect after stream is opened using open().
      @returns id variable that is binded to field pMember in relation pClass
    */
    BindedStid bindStid(const char* pTable, const char* pField, StoreableFilterBase* pFilter = NULL);
    /**
      Binds int value to database column.
      @param pTable database relation that we want to bind to
      @param pField column that variable will be binded to
      @param pFilter filter that converts database internal representation to int value. 
      @deprecated use bind instead
    */
    BindedInt bindInt(const char* pTable, const char* pField, StoreableFilterBase* pFilter = NULL);
    /**
      Binds string value to database column.
      @param pTable database relation that we want to bind to
      @param pField column that variable will be binded to
      @param pFilter filter that converts database internal representation to int value. 
      @deprecated use bind instead
    */
    BindedString bindString(const char* pTable, const char* pField, StoreableFilterBase* pFilter = NULL);
    /**
      Bind custom variable to stream.
      pFilter should be constructed using reference to variable that holds value that should be
      stored in database. Lifetime of variable have to be greater than stream where it is binded to.
      pType is a conversion hint.
      @param pTable name relation that owns pField
      @param pField name of field
      @param pFilter conversion filter that holds binded variable
      @param pType conversion hint for database and filter
    */
    void bind(const char* pTable, const char* pField, StoreableFilterBase* pFilter, Database::StoreType pType = Database::STRING);
    /**
      Prepare stream by creating member tree from pObject
      @param pObject object to examine
      @param pRootTable custom root table name
    */
    virtual void open(Storeable& pObject, const char* pRootTable = NULL);
    /**
      Change main table name. For input stream this must be called before open, for output stream if called before put 
      for object with NULL root table, it changes table name where object is stored
      @param pRootTable name of root table or NULL if name from store table should be used
    */
    void setRootTable(const char* pRootTable);
    /**
      Unbind all variables from stream. They will not be updated when getting next objects from stream
    */
    void unbindAll();
    /**
      Unbind all variables related to sql table
      @param pTableName name of table for variables to unbind      
    */
    void unbindAll(const char* pTableName);
    /**
      Unbind one variable
      @param pTableName name of table
      @param pField name of field      
    */
    void unbind(const char* pTableName, const char* pField);
    /**
      Check if stream is open
      @return true if stream is open, false otherwise.
    */
    virtual bool isOpen() const;
    /**
      Close stream
    */
    virtual void close() = 0;
    /**
      Destroy stream. After call to this function stream is destroyed
      and cannot be opened any more. It usually means that connection to database that
      this stream represents is closed after this method was called
    */
    virtual void destroy() = 0;
    /**
      Enable debugging and tracing for stream
    */
#ifdef _DEBUG
    void enableDebug(bool pFlag = true) { mDebugFlag = pFlag; };
#else
    void enableDebug(bool pFlag = true) {};
#endif
    /**
      Desctructor
    */  
    virtual ~Stream();
  protected:
    /**
      True if stream can use BIND_COL capability of Storeable object
      to load and store referenced objects
    */
    virtual bool isCollectionFilterSupported() const { return false; };
    /**
      Make object ok.
      @param pObject object that should be set to OK state
    */
    void makeOk(Storeable* pObject);
    /**
      alter object id.
      @param pObject object that should be set to OK state
      @param pNewId new id for object
    */
    void alterId(Storeable* pObject, int pNewId);
    /**
      Get number of tables from bottom of class hierarchy that are already stored in database
      @param pObject object to examine
      @return number of store tables
    */
    int getStoredTables(Storeable* pObject);
    /**
      Update filter pointer to actual pointer to member of Storeable object
      Used by applyFilter functions to set pointer to member before using filter.
      @param pFilter filter instance
      @param pMember pointer to member for filter
      @deprecated replaced by StoreableFilterBase::updateRef
    */
    static void setFilterPtr(StoreableFilterBase& pFilter, void* pMember);
    /**
      Get total number of store tables for object
      @param pObject object to examine
      @return number of store tables
    */
    int countTables(Storeable* pObject);
    /**
      Set number of tables that were stored in database 
      @param pObject object to update
      @param pTables new number of tables
    */
    void setStoredTables(Storeable* pObject,int pTables);
    /**
      Get root store table for object
      @param pObject object to examine 
    */
    const StoreTable* getTable(Storeable& pObject);
    /**
      Get name of root table for object
      @param pObj object to examine 
      @return root table name taken from mRootTable or topmost store table of passed object
    */
    const char* getRootTableName(Storeable& pObj);
    /**
      Get root collection table for object
      @param pObject object to examine 
    */
    const ColTable* getColTable(Storeable& pObject);
    /**
      Create member tree from store tables and binded variables
    */
    void createTree(const StoreTable* pRootTable);
#ifdef _DEBUG
    /**
      Stream debugging
    */
    void debug(const char* pFormat, ...);
#else
    void debug(const char* pFormat, ...) {};
#endif
    /**
      Data for binded variables
    */
    class dbaDLLEXPORT _bind_data {
      public:
        /**
          Constructor
          @param pTable table name
          @param pField field name from pTable
          @param pFilter filter that should be used to convert data
          @param pType conversion hint
        */
        _bind_data(const char* pTable, const char* pField, StoreableFilterBase* pFilter, Database::StoreType pType);
        /**Copy constructor*/
        _bind_data(const _bind_data& pObj);
        /**Assiment operator*/
        _bind_data& operator= (const _bind_data&);
        /**Destructor*/
        ~_bind_data();
        /**table name*/
        char* mTable;
        /**field name*/
        char* mField;
        /**converstion hint*/
        Database::StoreType mType;
        /**conversion filter*/
        shared_ptr<StoreableFilterBase> mFilter;
    };
    /**
      Relation between binded variable pointer and store table member identification.
      Used for unbind and unbindAll
    */
    class bindMatch {
      public:
        /**
          Constructor
          @param pPtr pointer to variable
        */
        bindMatch(void* pPtr);
        /**
          Constructor
          @param pTable relation name
          @param pField field name
        */
        bindMatch(const char* pTable, const char* pField);
        /**comparsion operator
          @param pData object to compare
        */
        bool operator() (const _bind_data& pData);
      private:
        void* mPtr;
        const char* mTable;
        const char* mField;
    };
    /**
      Type of list variables binded to stream
    */
    typedef std::list<_bind_data> VarMap;
    /**
      List of variables binded to stream
    */
    VarMap mBindings;
    /**
      List of members from store table
    */
    shared_ptr<mt_mlist> mMemberList;
    /**
      true if stream is open
    */
    bool mIsOpen;
    /**
      Name of overriden root table
    */
    const char* mRootTable;
#ifdef _DEBUG
    /**
      True if stream should output debug information
    */
    bool mDebugFlag;
#endif
  private:
    void unbindVar(void* pPtr);
};

}

#endif
