// File: ptr.h
// Purpose: shared pointers
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2001
// License: See COPYING file that comes with this distribution


#ifndef DBASHARED_PTR_H
#define DBASHARED_PTR_H

namespace dba {

/**
  shared_ptr implementation for streams
*/

class dbaDLLEXPORT Counter {
  public:
    Counter() : mCount(1) {};
    unsigned mCount;
};

template <class T> class shared_ptr {
  public:
    shared_ptr(T* pObj) : obj(pObj) {
      if (obj)
        mCounter = new Counter();
      else
        mCounter = NULL;
    };

    unsigned getCount() const {
      return mCounter->mCount;
    };

    shared_ptr<T>& operator= (T* pObj) {
      if (obj) {
        mCounter -> mCount--;
        if (!mCounter->mCount) {
          delete obj;
          delete mCounter;
        };
      };
      obj = pObj;
      if (obj) {
        mCounter = new Counter();
      };
      return *this;
    };

    shared_ptr() : obj(NULL),mCounter(NULL) {};

    shared_ptr(const shared_ptr& src) {
//      cout << "copy constructor of C" << endl;
      mCounter = src.mCounter;
      obj = src.obj;
      if (obj)
        mCounter -> mCount++;
    };

    shared_ptr<T>& operator= (const shared_ptr<T>& src) {
      if (obj) {
        mCounter -> mCount--;
          if (!mCounter->mCount) {
            delete obj;
            delete mCounter;
          };
      };
      mCounter = src.mCounter;
      obj = src.obj;
      if (obj)
        mCounter -> mCount++;
      return *this;
    };

    T& operator*() const {
      return *obj;
    };

    T* operator->() const {
      return obj;
    };

    bool operator!() const {
      if(obj)
        return false;
      return true;
    };

    bool operator==(const shared_ptr<T>& pPtr) const {
      return obj == pPtr.obj;
    };

    bool operator!=(const shared_ptr<T>& pPtr) const {
      return (!(operator==(pPtr)));
    };

    bool operator==(T* src) const {
      return (!(obj != src));
    };

    //Mainly for comparsion to NULL
    bool operator!=(T* src) const {
      if(obj == src)
        return false;
      return true;
    };

    T* ptr() const { return obj; }

    ~shared_ptr() {
//      cerr << "pointer destructor" << endl;
      if (obj) {
        mCounter -> mCount--;
        if (!mCounter -> mCount) {
          delete(obj);
          delete mCounter;
        };
      };
    };
  private:

    T* obj;
    Counter* mCounter;
};

};//namespace
#endif
