// File: collectionfilter.h
// Purpose: Base class for BIND_CLA and BIND_COL support filters
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBACOLLECTIONFILTER_H
#define DBACOLLECTIONFILTER_H

#include "dba/storeable.h"
#include <vector>
#include <memory>

namespace dba {

/**
  Iterator used for traversing on collection filter objects
  Every CollectionFilter derived class must provide implementation
  of this interface
  @ingroup filters
*/
class dbaDLLEXPORT CollectionFilterIterator {
  public:
    /**
      Constructor. Should create iterator pointing on first 
      object on list. If there are no objects then first call to hasNext()
      should return false.
    */
    CollectionFilterIterator() {};
    /**
      Get reference to object pointed by iterator.
    */
    virtual const Storeable& get() const = 0;
    /**
      Check if moveForward can be called.
      @return true if there are more objects to traverse, false otherwise
    */
    virtual bool hasNext() const = 0;
    /**
      Move iterator one object forward
    */
    virtual void moveForward() = 0;
    /**
      Destructor
    */
    virtual ~CollectionFilterIterator() {}
};

/**
  Base class for collection filters. This base is used by streams to interact
  with user collections binded using BIND_COL store table entry.
*/
class dbaDLLEXPORT CollectionFilterBase {
  public:
    /**
      Create new iterator for traversal.
      @return new iterator instance
    */
    virtual CollectionFilterIterator* createIterator() const = 0;
    /**
      Create or return reference to Storeable derived object
      for getting tables or loading from database.
      @return empty Storeable derived instance
    */
    virtual Storeable& create() = 0;
    /**
      Insert just loaded Storeable derived class to collection.
      Filters for lists implements this by adding reference to 
      Storeable object returned by create() to contained collection 
      @return pointer to inserted Storeable object
      @see InstanceFilter::put implmementation
    */
    virtual Storeable* put() = 0;
    /**
      Remove all objects from collection
    */
    virtual void clear() = 0;
    /**
      Find object with given id in collection.
      @param pKey id of object to find
    */ 
    virtual const Storeable& find(id pKey) {
      std::auto_ptr<CollectionFilterIterator> it(createIterator());
      while(it->hasNext()) {
        if (it->get().getId() == pKey)
          return it->get();
        it->moveForward();
      };
      throw APIException("parent object not found");
    };
    /**
      Get array of id values for all objects in collection
      @return vector of id values
    */
    virtual std::vector<id> getIds() {
      std::vector<id> ret;
      std::auto_ptr<CollectionFilterIterator> it(createIterator());
      while(it->hasNext()) {
        ret.push_back(it->get().getId());
        it->moveForward();
      };
      return ret;
    };
    /**
      Update reference to collection. Used by streams to adjust member before
      using this filter
      @param pNewMember pointer to new class member or variable
    */
    virtual void updateRef(void *pNewMember) = 0;
    /**
      Get pointer to collection
    */
    virtual void* getRef() const = 0;
    /**
      Destructor
    */
    virtual ~CollectionFilterBase() {}
};

/**
Filter for storing and retrieving 1-n and m-n relations 
from Archive into diffrent collection classes.
Implements part of CollectionFilterBase interface.
@ingroup filters
*/
template <typename M>
class CollectionFilter : public CollectionFilterBase {
  public:
    /**
      Constructor
      @param pMember refrence to collection that will be modified by filter
    */
    CollectionFilter(M& pMember) 
      : mMember(&pMember)
    {};
    virtual void updateRef(void *pNewMember) {
      mMember = (M*)pNewMember;
    };
    virtual void* getRef() const {
      return mMember;
    };
    virtual ~CollectionFilter() {};
  protected:
    /**
      pointer to collection to modify
      @warning can be changed by updateRef when processing data
    */
    M* mMember;
};

/**
  Base class for collection filters that use different class for collection 
  and for collection member. Implmements part of CollectionFilterBase interface.
  @ingroup filters
*/
template <typename M, typename S>
class InstanceFilter : public CollectionFilter<M> {
  public:
    /**
      Constructor
      @param pMember referecne to collection that filer will modify
    */
    InstanceFilter(M& pMember)
      : CollectionFilter<M>(pMember)
    {};
    /**
      Create new instance of Storeable dervied object of type S
      @return new instance
    */
    virtual Storeable& create() { return mInstance; };
    /**
      Add Storeable object to collection
      @param pToAdd object to add
      @return pointer to inserted Storeable object
    */
    virtual Storeable* add(const S& pToAdd) = 0;
    virtual Storeable* put() {
      return add(mInstance);
    };
    virtual ~InstanceFilter() {}
  private:
    S mInstance;
};

}

#endif
