// File: stddeque.h
// Purpose: Collection filter for std::deque
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDDEQUE_H
#define DBASTDDEQUE_H

#include "dba/collectionfilter.h"
#include <deque>

namespace dba {

/**
  Collection filter for std::deque
  @ingroup filters
*/
template <typename T>
class stdDeque  : public InstanceFilter<std::deque<T>, T> {
  public:
    /**@internal
      Collection iterator implementation for std::vector collection filter
    */
    class iterator : public CollectionFilterIterator {
      public:
        iterator(std::deque<T>& pDeque) 
          : mDeque(pDeque)
        { 
          //std::cerr << "stdSet iterator for set " << &mSet << std::endl;
          it = mDeque.begin(); 
        };
        virtual const Storeable& get() const { 
          //std::cerr << "returning obj "<< &(*it) << " [" << it->getId() << "] from set " << &mSet << std::endl;
          return *it; 
        };
        virtual bool hasNext() const { return it != mDeque.end(); };
        virtual void moveForward() { 
          //std::cerr << "moving forward on set " << &mSet << std::endl;
          ++it; 
        };
      private:
        typename std::deque<T>::iterator it;
        typename std::deque<T>& mDeque;
    };
    /**
      Constructor
      @param pDeque object that will be modified by filter
    */
    stdDeque(std::deque<T>& pDeque) : InstanceFilter<std::deque<T>, T>(pDeque) {};
    virtual CollectionFilterIterator* createIterator() const { 
      return new iterator(*(CollectionFilter<std::deque<T> >::mMember)); 
    };
    virtual void clear() { (CollectionFilter<std::deque<T> >::mMember)->clear(); };
    virtual Storeable* add(const T& pToAdd) {
      //std::cerr << "adding obj "<< pToAdd.getId() << " to set " << mMember << std::endl;
      (CollectionFilter<std::deque<T> >::mMember)->push_back(pToAdd);
      return &((CollectionFilter<std::deque<T> >::mMember)->back());
    };
    virtual ~stdDeque() {};
};

} //namespace

#endif
