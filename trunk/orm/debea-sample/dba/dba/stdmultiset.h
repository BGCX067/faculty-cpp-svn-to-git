// File: stdmultiset.h
// Purpose: Collection filter for std::multiset
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDMULTISET_H
#define DBASTDMULTISET_H

#include "dba/collectionfilter.h"
#include <set>

namespace dba {

/**
Collection filter for std::multiset
@ingroup filters
*/
template <typename T>
class stdMultiset  : public InstanceFilter<std::multiset<T>, T> {
  public:
    /**@internal
      Collection iterator implementation for std::multiset collection filter
    */
    class iterator : public CollectionFilterIterator {
      public:
        iterator(std::multiset<T>& pSet) 
          : mSet(pSet)
        { 
          //std::cerr << "stdlist iterator for set " << &mSet << std::endl;
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
        typename std::multiset<T>::iterator it;
        typename std::multiset<T>& mSet;
    };
    /**
      Constructor
      @param pList object that will be modified by filter
    */
    stdMultiset(std::multiset<T>& pList) : InstanceFilter<std::multiset<T>, T>(pList) {};
    virtual CollectionFilterIterator* createIterator() const { 
      return new iterator(*(CollectionFilter<std::multiset<T> >::mMember)); 
    };
    virtual void clear() { (CollectionFilter<std::multiset<T> >::mMember)->clear(); };
    virtual Storeable* add(const T& pToAdd) {
      //std::cerr << "adding obj "<< pToAdd.getId() << " to set " << mSet << std::endl;
      typename std::multiset<T>::iterator it = (CollectionFilter<std::multiset<T> >::mMember)->insert(pToAdd);
      return (Storeable*)&(*it);
    };
    virtual ~stdMultiset() {};
};

}

#endif
