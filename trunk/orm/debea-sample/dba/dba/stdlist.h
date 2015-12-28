// File: stdlist.h
// Purpose: Collection filter for std::list
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDLIST_H
#define DBASTDLIST_H

#include "dba/collectionfilter.h"
#include <list>

namespace dba {

/**
Collection filter for std::list
@ingroup filters
*/
template <typename T>
class stdList  : public InstanceFilter<std::list<T>, T> {
  public:
    /**@internal
      Collection iterator implementation for std::list collection filter
    */
    class iterator : public CollectionFilterIterator {
      public:
        iterator(std::list<T>& pList) 
          : mList(pList)
        { 
          //std::cerr << "stdlist iterator for list " << &mList << std::endl;
          it = mList.begin(); 
        };
        virtual const Storeable& get() const { 
          //std::cerr << "returning obj "<< &(*it) << " [" << it->getId() << "] from list " << &mList << std::endl;
          return *it; 
        };
        virtual bool hasNext() const { return it != mList.end(); };
        virtual void moveForward() { 
          //std::cerr << "moving forward on list " << &mList << std::endl;
          ++it; 
        };
      private:
        typename std::list<T>::iterator it;
        typename std::list<T>& mList;
    };
    /**
      Constructor
      @param pList object that will be modified by filter
    */
    stdList(std::list<T>& pList) : InstanceFilter<std::list<T>, T>(pList) {};
    virtual CollectionFilterIterator* createIterator() const { 
      return new iterator(*(CollectionFilter<std::list<T> >::mMember)); 
    };
    virtual void clear() { (CollectionFilter<std::list<T> >::mMember)->clear(); };
    virtual Storeable* add(const T& pToAdd) {
      //std::cerr << "adding obj "<< pToAdd.getId() << " to list " << mMember << std::endl;
      (CollectionFilter<std::list<T> >::mMember)->push_back(pToAdd);
      return &((CollectionFilter<std::list<T> >::mMember)->back());
    };
    virtual ~stdList() {};
};

}

#endif
