// File: storeablelist.h
// Purpose: first idea how to manage std::list of dba::Storeable objects (depreciated)
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASTOREABLELIST_H
#define DBASTOREABLELIST_H

#include <algorithm>
#include <list>
#include <set>

#include "dba/storeable.h"

namespace dba {

/**
  Functor for std::find and std::find_if
  @ingroup api
*/
struct idMatch {
  /**
    Construtor
    @param pId id of Storeable object
  */
  idMatch(id pId) : mId(pId) {};
  /**
    Comparsion operator
    @param pObj object to compare
  */
  bool operator() (const dba::Storeable& pObj) const {
    return mId == pObj.getId();
  };
  private:
    id mId;
};

#ifdef DBA_COMPAT_1_0
/**@brief STL list for Storeable objects
  */
template <typename T> class StoreableList : public std::list<T> {
  public:
    static void dump(const StoreableList<T>& pList);
    StoreableList() {};
    StoreableList(const typename std::list<T>& pList) {
      for (typename std::list<T>::const_iterator it = pList.begin(); it != pList.end();it++)
        push_back(*it);
    };
    StoreableList& operator=(const StoreableList& pSource);
//    StoreableList(const StoreableList<T>& pSource);
    bool update(const typename std::list<T>& pNew);
    std::list<T> filterDeleted();
    //typename std::list<T>::iterator setDeleted(const T& pObject); 

    void setState(dba::Storeable::stState pState);
    void lockStid(bool pForce);
    void unlockStid();
    void eraseNew();
    void eraseDeleted();
    bool add(const typename std::list<T>& pFrom);
    //!erase subset of objects using id
    bool eraseById(const std::set<int>& pIds);
};

template <typename T> StoreableList<T>&
StoreableList<T>::operator=(const StoreableList& pSource) {
  if (this != &pSource) {
    update(pSource);
  };
  return *this;
};

/**
  * This function compares two lists of storeable elements.
  * All elements on mList list are replaced with elements on pNew list.
  * Elements are considered equal if their id matches.
  * ALL NEW ELEMENTS ARE ERASED BY THIS METHOD
  * Elements that are not on pNew lists and are still on mList lists
  * will get state Storeable::DELETED after comparsion
  * @warning mList list mus be synced (must not contains objects in STOREABLE::NEW/DELETED state!
  * @return true if at least one element was replaces, false otherwise.
  */
template <typename T> bool
StoreableList<T>::update(const typename std::list<T>& pNew) {
  //"nothing changed" flag;
  bool ret = false;
  eraseNew();
  //make them all deleted
  setState(dba::Storeable::DELETED);
  //Add all elements with NEW state to container.
  //Update all other elements by assigning or adding them if they are no already on list
  for(typename std::list<T>::const_iterator newit = pNew.begin(); newit != pNew.end(); newit++) {
    bool processed = false;
    if (newit -> isNew()) {
      push_back(*newit);
      ret = true;
      processed = true;
    } else {
      for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++) {
        if (newit -> getId() == it -> getId()) {
          ret = true;
          *it = *newit;
          processed = true;
        };
      };
      //element not found on list
      if (!processed) {
        ret = true;
        processed = true;
        push_back(*newit);
      };
    };
  };
  return ret;
};

/**
  This method adds all elements from pFrom to list
  If element being added is already on Storeable list it will be updated
  If there is no such element it woll be added.
  Comparsion is done using Storeable::mId
  All new elements are added by default
*/

template <typename T> bool
StoreableList<T>::add(const typename std::list<T>& pFrom) {
  bool ret = false;
  for(typename std::list<T>::const_iterator newit = pFrom.begin(); newit != pFrom.end(); newit++) {
    if (newit -> isNew()) {
      ret = true;
      push_back(*newit);
    } else {
      bool processed = false;
      for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++) {
        if (newit -> getId() == it -> getId()) {
          processed = true;
          ret = true;
          *it = *newit;
        };
      };
      if (!processed) {
        push_back(*newit);
      };
    };
  };
  return ret;
};


template <typename T> void
StoreableList<T>::eraseDeleted() {
  typename std::list<T>::iterator it = std::list<T>::begin();
  while(it != std::list<T>::end()) {
    if (it -> isDeleted()) {
      typename std::list<T>::iterator e = it++;
      erase(e);
    } else {
      it++;
    };
  };
};

template <typename T> void
StoreableList<T>::eraseNew() {
  typename std::list<T>::iterator it = std::list<T>::begin();
  while(it != std::list<T>::end()) {
    if (it -> isNew()) {
      typename std::list<T>::iterator e = it++;
      erase(e);
    } else {
      it++;
    };
  };
};


/*template <class T> void
StoreableList::remove(list<T>& pList, list<T>::iterator& it) {
  if (!it -> isDeleted())
    return;
  list<T>::iterator e = it++;
  pList.erase(e);
};*/

template <typename T> void
StoreableList<T>::setState(dba::Storeable::stState pState) {
  for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++)
    it -> setState(pState);
};

template <typename T> std::list<T>
StoreableList<T>::filterDeleted() {
  std::list<T> ret;
  for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++)
    if (!it -> isDeleted())
      ret.push_back(*it);
  return ret;
};

template <typename T> bool
StoreableList<T>::eraseById(const std::set<int>& pIds) {
  bool changed = false;
  for(typename std::set<int>::const_iterator it = pIds.begin(); it != pIds.end(); it++) {
    typename std::list<T>::iterator e = find_if(std::list<T>::begin(), std::list<T>::end(),idMatch(*it));
    if (e != std::list<T>::end()) {
      erase(e);
      changed = true;
    };
  };    
  return changed;
};

template <typename T> 
void 
StoreableList<T>::lockStid(bool pForce) {
  for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++)
    it->lockStid(pForce);
};

template <typename T> 
void 
StoreableList<T>::unlockStid() {
  for(typename std::list<T>::iterator it = std::list<T>::begin(); it != std::list<T>::end(); it++)
    it->unlockStid();
};

#endif

};//namespace

#endif
