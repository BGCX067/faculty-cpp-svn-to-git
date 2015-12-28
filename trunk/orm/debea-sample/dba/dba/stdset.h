// File: stdset.h
// Purpose: Collection filter for std::set
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDSET_H
#define DBASTDSET_H

#include "dba/collectionfilter.h"
#include <set>

namespace dba {

/**
  Collection filter for std::set
  Objects that are stored using this collection should not implement 
  operator\< using Storeable fields because they will be modified directly
  in std::set.

  When updating object in set remember that you need to preserve dba::Storeable 
  state. This means that you need to remove old one, update fields of old with new 
  and readd old one again like this:
@code
  //having a set like this
  SomeObject old, newone;
  std::set<SomeObject> objset;
  objset.insert(old);

  //update object in the set this way:
  Object old(objset.erase(objset.begin());
  dba::IdLocker l;
  old = newone;
  exset.insert(old);
@endcode
  @ingroup filters
*/
template <typename T>
class stdSet  : public InstanceFilter<std::set<T>, T> {
  public:
    /**@internal
      Collection iterator implementation for std::set collection filter
    */
    class iterator : public CollectionFilterIterator {
      public:
        iterator(std::set<T>& pSet) 
          : mSet(pSet)
        { 
          //std::cerr << "stdSet iterator for set " << &mSet << std::endl;
          it = mSet.begin(); 
        };
        virtual const Storeable& get() const { 
          //std::cerr << "returning obj "<< &(*it) << " [" << it->getId() << "] from set " << &mSet << std::endl;
          return *it; 
        };
        virtual bool hasNext() const { return it != mSet.end(); };
        virtual void moveForward() { 
          //std::cerr << "moving forward on set " << &mSet << std::endl;
          ++it; 
        };
      private:
        typename std::set<T>::iterator it;
        typename std::set<T>& mSet;
    };
    /**
      Constructor
      @param pSet object that will be modified by filter
    */
    stdSet(std::set<T>& pSet) : InstanceFilter<std::set<T>, T>(pSet) {};
    virtual CollectionFilterIterator* createIterator() const { 
      return new iterator(*(CollectionFilter<std::set<T> >::mMember)); 
    };
    virtual void clear() { (CollectionFilter<std::set<T> >::mMember)->clear(); };
    virtual Storeable* add(const T& pToAdd) {
      //std::cerr << "adding obj "<< pToAdd.getId() << " to set " << mMember << std::endl;
      typename std::pair<typename std::set<T>::const_iterator,bool> p = (CollectionFilter<std::set<T> >::mMember)->insert(pToAdd);
      return (Storeable*)&(*(p.first));
    };
    virtual ~stdSet() {};
};

} //namespace

#endif
