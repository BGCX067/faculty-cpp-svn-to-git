// File: stdvector.h
// Purpose: Collection filter for std::vector
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTDVECTOR_H
#define DBASTDVECTOR_H

#include "dba/collectionfilter.h"
#include <vector>

namespace dba {

/**
  Collection filter for std::vector
  @ingroup filters
*/
template <typename T>
class stdVector  : public InstanceFilter<std::vector<T>, T> {
  public:
    /**@internal
      Collection iterator implementation for std::vector collection filter
    */
    class iterator : public CollectionFilterIterator {
      public:
        iterator(std::vector<T>& pVector) 
          : mVector(pVector)
        { 
          //std::cerr << "stdSet iterator for set " << &mSet << std::endl;
          it = mVector.begin(); 
        };
        virtual const Storeable& get() const { 
          //std::cerr << "returning obj "<< &(*it) << " [" << it->getId() << "] from set " << &mSet << std::endl;
          return *it; 
        };
        virtual bool hasNext() const { return it != mVector.end(); };
        virtual void moveForward() { 
          //std::cerr << "moving forward on set " << &mSet << std::endl;
          ++it; 
        };
      private:
        typename std::vector<T>::iterator it;
        typename std::vector<T>& mVector;
    };
    /**
      Constructor
      @param pVector object that will be modified by filter
    */
    stdVector(std::vector<T>& pVector) : InstanceFilter<std::vector<T>, T>(pVector) {};
    virtual CollectionFilterIterator* createIterator() const { 
      return new iterator(*(CollectionFilter<std::vector<T> >::mMember)); 
    };
    virtual void clear() { (CollectionFilter<std::vector<T> >::mMember)->clear(); };
    virtual Storeable* add(const T& pToAdd) {
      //std::cerr << "adding obj "<< pToAdd.getId() << " to set " << mMember << std::endl;
      (CollectionFilter<std::vector<T> >::mMember)->push_back(pToAdd);
      return &((CollectionFilter<std::vector<T> >::mMember)->back());
    };
    virtual ~stdVector() {};
};

} //namespace

#endif
