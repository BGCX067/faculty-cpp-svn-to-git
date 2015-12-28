// File: single.h
// Purpose: Collection filter for storing 1-1 relations
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASINGLE_H
#define DBASINGLE_H

#include "dba/collectionfilter.h"

namespace dba {

/**
Collection filter for 1-1 relation.
  @ingroup filters
*/
class dbaDLLEXPORT Single : public CollectionFilter<Storeable> {
  public:
    class iterator : public CollectionFilterIterator {
      public:
        iterator(Storeable& pObject) 
          : mObject(pObject)
        { mMoveCalled = false; }
        virtual const Storeable& get() const { return mObject; };
        virtual bool hasNext() const { return !mMoveCalled; };
        virtual void moveForward() { mMoveCalled = true; };
      private:
        bool mMoveCalled;
        Storeable& mObject;
    };
    /**
      Constructor
      @param pObj object to be updated
    */
    Single(Storeable& pObj) : CollectionFilter<Storeable>(pObj) {};
    virtual CollectionFilterIterator* createIterator() const { return new iterator(*mMember); };
    virtual Storeable& create() { return *mMember; };
    //iterator is assiging us
    virtual Storeable* put() { return mMember; }
    virtual void clear() {};
    ~Single() {};
};

}

#endif
