// File: stlutils.h
// Purpose: Tools for managing dba::Storeable objects on STL collections (depreciated)
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBASTLUTILS_H
#define DBASTLUTILS_H

#include <list>
#include <assert.h>
#include <algorithm>
#include "dba/storeable.h"
#include "dba/idlocker.h"

namespace dba {

/**@internal 
  Helper function for STLUtils::setDeleted
*/
class makeDeleted {
  public:
    void operator() (Storeable& pObj) {
      pObj.setDeleted();
    };
};

/**
@ingroup api
*/
class dbaDLLEXPORT STLUtils {
  public:
    /**
      Remove all elements in DELETED state from pList
      @param pList object to modify
    */
    template <typename T> static void eraseDeleted(std::list<T>* pList);
    /**
      Remove all elements in NEW state from pList
      @param pList object to modify
    */
    template <typename T> static void eraseNew(std::list<T>* pList);
    /**
      Set all elements to DELETED state on pList
      @param pList object to modify
    */
    template <typename T> static void setDeleted(std::list<T>* pList);
    /**
      Update pOld list from pNew list.
      This function compares two lists and change states of elements according to its contents.
      If element from pOld list is not on pNew list then make it DELETED.
      If element on pNew list is not on pOld list then it is added to pOld list with NEW state
      If element from pOld list (O) is on pNew list (N) then its state is determined by using operator==. If O == N then O is untouched, else N is assigned to O and 
      state of O becomes CHANGED.
      
      Elements are matched using external comparator.
      @param pOld source list
      @param pNew destination list
      @param pIdentityComparator object used for finding instance from old list on new list
      @param pContentsComparator objecct used to compare instance from old list with one on new list
    */
    template <typename T, typename I, typename C> static void sync(std::list<T>* pOld, const std::list<T>& pNew, const I& pIdentityComparator, const C& pContentsComparator);
};

template <typename T>
void
STLUtils::eraseDeleted(std::list<T>* pList) {
  typename std::list<T>::iterator it = pList->begin();
  while(it != pList->end()) {
    if (!it->isDeleted())
      it++;
    else
      it = pList->erase(it);
  };
};

template <typename T>
void
STLUtils::setDeleted(std::list<T>* pList) {
  std::for_each(pList->begin(),pList->end(),makeDeleted());
};


template <typename T>
void
STLUtils::eraseNew(std::list<T>* pList) {
  typename std::list<T>::iterator it = pList->begin();
  while(it != pList->end()) {
    if (!it->isNew())
      it++;
    else
      it = pList->erase(it);
  };
};

template <typename T, typename I, typename C> 
void
STLUtils::sync(std::list<T>* pOld, const std::list<T>& pNew, const I& pIdentityComparator, const C& pContentsComparator) {
  I icmp(pIdentityComparator);
  C ccmp(pContentsComparator);
  assert(IdLocker::isUnlocked());
  std::list<T> result;
  for(typename std::list<T>::const_iterator newit = pNew.begin(); newit != pNew.end(); newit++) {
    icmp.set(*newit);
    typename std::list<T>::iterator oldit = find_if(pOld->begin(), pOld->end(), icmp);
    if (oldit != pOld->end()) {
      //update of old element, preserve id and set new values
      //only if element is different from original
      if (!ccmp.contentChanged(*oldit,*newit)) {
        result.push_back(*oldit);
      } else {
        T x(*oldit);
        {
          IdLocker l;
          x = *newit;
        };
        result.push_back(x);
      };
      pOld->erase(oldit);
    } else {
      //new element, preserve NEW state and set values from list
      IdLocker l;
      T x(*newit);      
      result.push_back(x);
    };
  };    
  //pOld now contains only elements that should be in deleted state
  //add them with deleted state  
  for(typename std::list<T>::iterator oldit = pOld->begin(); oldit != pOld->end(); oldit++) {
    if (!oldit->isNew()) {
      oldit->setDeleted();
      result.push_back(*oldit);
    };
  };
  *pOld = result;    
};


}

#endif
