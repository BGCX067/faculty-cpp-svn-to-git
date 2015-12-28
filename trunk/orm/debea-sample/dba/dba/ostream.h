// File: ostream.h
// Purpose: Base class for archive output streams
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAOSTREAM_H
#define DBAOSTREAM_H

#include <vector>
#include "dba/stream.h"

namespace dba {

/**
  OStream represents Object Stream to Archive. You can put any class that
  inherits from Storeable class into OStream.
  @ingroup api
*/
class dbaDLLEXPORT OStream : public Stream {
  public:
    /**
      Constructor
    */
    OStream();
    //!Open stream. If stream is opened no vars can be binded anymore.
    /**
      Create transaction on stream. Changes in database will be all commited only after
      calling close().
    */
    virtual void begin() = 0;
    /**
      Commit all changes made to stream from point where begin() was called.
      @note You should use dba::Transaction object instead of calling this method
    */
    virtual void commit() = 0;
    /**
      Cancel all changes to stream from point where begin() was called.
      @note You should use dba::Transaction object instead of calling this method
    */
    virtual void rollback() = 0;
    /**
      Put object into stream and update all data from binded variables that are binded to rows
      in database that are affected by putting pObject.
      
      Depending on state of pObject store, erase or update is called.
      @note You should use dba::Transaction object instead of calling this method
      @param pObject object to store
    */
    virtual bool put(Storeable* pObject);
    /**
      Prepare stream to put Storeable objects in it. If there is problem with object store then dba::DataException is thrown. 
      If there is problem with database connection then dba::DatabaseExcption is thrown.
      @param pRootTable new root relation name for each Storeable object that is put into stream. If NULL then \
      name from store table is used
      @return true if at least one object was stored or erased in database, false if no store was performed (all objects are in OK state)
    */
    virtual void open(const char* pRootTable = NULL);
    /**
      Assign object identifier.
      @param pObject Object which id should be set. This object must not have its id set
    */
    virtual void assignId(Storeable* pObject) throw (Exception) = 0;
    /**
      Destructor
    */
    virtual ~OStream() {};
  protected:
    virtual void initMemberChildrenStore(const Storeable& pObject, const ColMemberEntry& pMember) {};
    virtual void endMemberChildrenStore(const Storeable& pObject, const ColMemberEntry& pMember) {};
    /**
      Default implementation for BIND_COL store that works on data stored in relations and 
      linked using foreign keys. For data formats that are not layed out in this kind of structures
      you should override this method providing different alghoritm
      @param pObject pointer to root object
      @param pMember member from collection table
      @param pFilter filter that should be used to create instance of object
      @param pTableName name of root relation
      @return true if at least one subobject object was stored in archive, false otherwise
    */
    virtual bool putMemberChildren(Storeable* pObject, ColMemberEntry& pMember, CollectionFilterBase& pFilter, const char* pTableName);
    /**
      Store all Storeable objects on sublists listed as BIND_COL in store table
      @param pObject object that will be examined
    */
    virtual bool putChildren(Storeable* pObject);
    /**
      Erase all objects that are related to Storeable pObj object using
      BIND_COL entries in store table.
      @param pRefData list of object identifiers
      @param pObj instance of object that will be erased. Used for extractiin ColTable recursively
      @param pRootTableName altered name of root store table of pObj or NULL if root table was not altered
    */
    virtual bool eraseChildren(const std::vector<id>& pRefData, Storeable& pObj, const char* pRootTableName);
  private:
    /**
      Remove object from stream. After call to this function pObject should be removed from database or file
      @param pObject object to remove.
    */
    virtual bool erase(Storeable* pObject) = 0;
    /**
      Update object in stream. After call to this function pObject should be updated in database or file
      @param pObject object to remove.
    */
    virtual bool update(Storeable* pObject) = 0;
    /**
      Create object in stream. After call to this function pObject should be created in database or file
      @param pObject object to remove.
    */
    virtual bool store(Storeable* pObject) = 0;
    /**
      Load reference data for updating children of Storeable object. 
      Used by putChildren. If stream supports CollectionFilters
      then it mus implement this method. Default implementation throws APIException
      @param pTable name of %SQL table
      @param pFkName name of foreign key
      @param pCollId collection identifier or Storeable::InvalidId if there is no identifier
      @param pId id of parent object
    */
    virtual std::vector<id> loadRefData(const char* pTable, const char* pFkName, id pCollId, id pId);
    /**
      Delete referenced objects of Storeable object. 
      Used by putChildren. If stream supports CollectionFilters
      then it mus implement this method. Default implementation throws APIException
    */
    virtual bool deleteRefData(const std::vector<id>& pIds, const char* pTableName);
};

#ifdef DBA_COMPAT_1_0
typedef OStream OOStream;
#endif

};

#endif
