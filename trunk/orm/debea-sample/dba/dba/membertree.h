// File: membertree.h
// Purpose: Utilities for class member tables 
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2002
// License: See COPYING file that comes with this distribution

#ifndef DBAMEMBERTREE_H
#define DBAMEMBERTREE_H

#include "dba/defs.h"

namespace dba {

/**
  This struct descrbes class member.
  */
struct dbaDLLEXPORT mt_member {
  //Structure members.
  //!Offset from address of object to member
  int offset;
  //!name of coresponding field in %SQL table
  const char* name; 
  //!list pointer
  mt_member* next;

  //Data members.
  /**
    filter function (@see Storeable class filters)
  */
  void* func;
  //Database::type enum, that tells DbResult object, that we will get field from it as String, Int,
  //Date or double. \see Database class
  int type;
};

/**
  This struct describes class entry in mlist. Every class in inheritance hierarchy has one mt_table entry
  in mt_list structure.
*/
struct dbaDLLEXPORT mt_class {
  const char* name;
  mt_class* next;
  mt_member* firstField;
};

/**structure for representing offsets to members of classes
inside parent hierarhy tree.
This class is mainly created for implementing EVENT, MESSAGE and DBSTORAGE tables
<pre>
mt_list
  ^
  |
mt_class -> mt_class -> mt_class -> NULL
               ^
               |
            mt_member -> mt_member -> mt_member -> NULL

</pre>

Leafs (mt_class and mt_member objects) of this structure are allocated only once. Each leaf can be full, or empty.
Leaf is empty if his name points to NULL, and full if its name points to something


If leaf is empty it can be filled by add() methods.
If leaf is full it can be cleared by clear() methods.

Destructor frees memory allocated for structure.

*/
class dbaDLLEXPORT mt_mlist {
  public:
    mt_mlist();
    /**
      Adds class entry for table if it is not already in
    */
    void addClass(const char* table);
    /**
      Adds new member to structure. Class entry is created if needed.
    */
    void add(const char* table, const char* field,int offset,void* f,int type);
    void add(mt_class* t, const char* name, int offset,void* f, int type);
    /**
      @return func pointer for first offset found or NULL if offset was not in structure
      */
    void* findFuncByOffset(int pOffset) const;
    /**
      @return func pointer for first offset found or NULL if offset was not in single class list
      */
    void* findFuncByOffset(mt_class* pTable, int pOffset) const;
    /**
      @return pointer to member or NULL if member is not in structure
    */
    mt_member* findMember(const char* pClassName, const char* pFieldName) const;
    /**
      @return pointer to member or NULL if member is not in single class list
    */
    mt_member* findMember(mt_class* pTable, const char* pField) const;
    int getSize() const;
    void clear();
    void clear(mt_class* t);
    ~mt_mlist();

    mt_class* mFirst;
    int qb_size;
  private:
    mt_mlist(mt_mlist& pList);
    void operator=(const mt_mlist& pList);
};

};//namespace

#endif
