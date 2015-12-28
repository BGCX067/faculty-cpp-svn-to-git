// File: idlocker.h
// Purpose: Support for cloning objects
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2005
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBAIDLOCKER_H
#define DBAIDLOCKER_H

#include "dba/defs.h"

namespace dba {

/**
Helper class to lock id of object when using assigment operator

Id of all objects are locked until at least one locker exists, so this object should be created only on stack.

Example of use:
@code
Storable a; //new object
Storeable b = loadFromDatabase(); //object with id

IdLocker locker;
b = a; // b still has its id
@endcode

@ingroup api
*/
class dbaDLLEXPORT IdLocker {
  public:
    /**
      Lock mode
    */
    typedef enum {
      UNLOCKED = 0, //!Storeable object states are copied
      LOCKED = 1, //!Storeable object states are not copied
      LOCKED_PRESERVE_STATE = 2 //!Storeable object state is not copied, id is copied
    } mode;
    /**
      Constructor
      @param pMode modify behaviour of Storeable copy constructor and assigment operator
    */
    IdLocker(mode pMode = LOCKED);
    /**
      Check if locker is not in UNLOCKED state
      @return true if locker is not in UNLOCKED state, false otherwise
    */
    static bool isLocked();
    /**
      Check if locker is in UNLOCKED state
      @return true if locker is in UNLOCKED state, false otherwise
    */
    static bool isUnlocked();
    /**Destructor*/
    ~IdLocker();
  private:
};

}

#endif
