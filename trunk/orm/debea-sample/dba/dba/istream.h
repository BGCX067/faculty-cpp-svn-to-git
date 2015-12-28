// File: istream.h
// Purpose: Base class for archive istreams
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAISTREAM_H
#define DBAISTREAM_H

#include "dba/stream.h"
#include <vector>

namespace dba {

/**
  IStream represents Object Input stream from Archive. IStream can be opened for any
  class that inherits from Storeable class. Notice that unlike Output stream, Input stream
  can be opened only for single class so you cannot get objects of different classes
  using getNext() from one stream. To do this, you have to reopen stream for new class 
  that you want to load.

  Before opening stream variables to additional columns can be binded. Then when stream  
  is opened query is constructed and send (or data is prepared in other way if backend database
  is not %SQL based database). You can get objects by creating empty object and calling get()
  on that object. After that object data members and all binded vars are updated by values
  from query result.

  If you use getNext(), then there is also possibility to updateVars without updating object. 
  This is useful if you are manually reconstructing referenced to other objects. 
  Then you want to get variables first, find referenced objects and and then create object using 
  getNext().
  
  As getNext is useful when fetching single Storeable objects, you can use get() to load 
  particular Storeable derived object and all subobjects that are specified using BIND_COL 
  or BIND_CLA macros.

  You can open stream by using Stream::open() method.
  @ingroup api
*/

class dbaDLLEXPORT IStream : public Stream {
  public:
    /**
      Constructor
    */
    IStream();
    /**
      Get next object from stream.
      @param pObject object to be updated from stream
      @return true if object was retrieved or false if there is no more data in stream
    */
    virtual bool getNext(Storeable* pObject) = 0;
    /**
      Get single Storeable derived object from database. 
      If id of object is not set then random one is retrieved.
      Stream must not be open for any other object to call this method.

      If pRootTable is specified then it overrides name of root table of last 
      store table.

      All objects in collections binded by BIND_COL are erased before collections
      are filled with database records.

      @param pObject object to retrieve
      @param pRootTable ovverided name of relation to retrieve from 
    */
    virtual bool get(Storeable* pObject, const char* pRootTable = NULL);
    /**
      Get list of objects from database with its relations.
      You should use adequate filter for given collection:
@code
std::list<MyObject> lst;
dba::IStream istream(archive->getIStream());
dba::stdList<MyObject> filter(lst);
istream.get(&filter);
@endcode
      For retrieving subset of data you should use setWherePart to
      set condition for retieval.
      @param pFilter CollectionFilter that contains Storeable object to modify 
      @param pRootTable name of relation of root Storeable object passed to pFilter.
    */
    virtual bool get(CollectionFilterBase* pFilter, const char* pRootTable = NULL);
    /**
      Update binded vars without retrieving current object
      @return true vars were updated or false if there is no more data in stream
    */
    virtual bool updateVars() = 0;
    /**
      Destructor
    */
    virtual ~IStream() {};
  protected:
    /**
      configure stream to open for records that have one of values from array
      in field named pFKeyName. Used by getChildren to retrieve reference data 
      for BIND_COL collections.
      @param pFKeyName name of foreign key field
      @param pRelationId collection identification from BIND_CLA or Storeable::Invalid if not used
      @param pIds values of foreign key 
    */
    virtual void setIdsCondition(const char* pFKeyName, id pRelationId, const std::vector<id>& pIds);
  private:
    /**
      Recursive retrieval of BIND_COL collections
      @param pFilter filter that contains Storeable object to modify
      @param pMember pointer to collection that will be filled with children
      @param pParentIds list of parent id values for setIdsCondition
    */
    bool getChildren(CollectionFilterBase* pFilter, const ColTable* pTable, const std::vector<id>& pParentIds);
};

#ifdef DBA_COMPAT_1_0
typedef IStream OIStream;
#endif

}

#endif
