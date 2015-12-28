// File: storeable.h
// Purpose: Object that can be serialized by archive
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2002
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBASTOREABLE_H
#define DBASTOREABLE_H

/** @file storeable.h*/ 

#include <string>
#include "dba/database.h"
#include "dba/archiveexception.h"
#include "dba/storeablefilter.h"
#include "dba/defs.h"

namespace dba {

/**
  Base class for store and collection table members
*/
class dbaDLLEXPORT MemberEntryBase {
  public:
    /**@internal
      Constructor - for internal use only
      @param pMemberName name of relation field for member
      @param pMemberOffset number of bytes from 'this' to first byte of member field in class
    */
    MemberEntryBase(const char* pMemberName, int pMemberOffset);
    /**@internal
      Get number of bytes from this to first byte of member field
    */
    int getMemberOffset() const;
    /**
      Get name of relation where class member is stored
    */
    const char* getMemberName() const;
    /**@internal
      set in BIND_* macros - for internal use only
    */
    void setMemberOffset(int pNewOffset);
  private:  
    const char* mMemberName;
    int mMemberOffset;
};

template <typename T, typename M>
class MemberTableBase {
  public:
    MemberTableBase(const char* pTableName);
    const char* getTableName() const;
    int getClassOffset() const;
    void setClassOffset(int pOffset);
    const T* getNextTable() const;
    M* getMembers() const;
    void addMember(M* pMember);
    void setNextTable(const T* pTable);
    ~MemberTableBase();
  private:
    int mClassOffset;
    const char* mTableName;
    const T* mNextTable;
    M* mMembers;
};

class StoreTable;
class ColTable;

/**
  %Single entry in store table
*/
class dbaDLLEXPORT StoreTableMember : public MemberEntryBase {
  public:
    /**@internal
      Constructor - used by BIND_* macros - for internal use only
      @param pOwner store table that owns this member
      @param pMemberName name of relation field for member
      @param pMemberOffset number of bytes from 'this' to first byte of member field in class
      @param pFilter instance of storeable filter for data conversion
      @param pDatabaseType type of archive data (Database::StoreType enum value)
    */
    StoreTableMember(StoreTable* pOwner, const char* pMemberName, int pMemberOffset, StoreableFilterBase* pFilter, int pDatabaseType);
    /**@internal
      used by BIND_* macros - for internal use only
      @param pFlag true if member owns passed pointer
    */
    void setFilterOwner(bool pFlag);
    /**@internal
      Add member to store table - for internal use only
      @param pOwner store table where member should be added
    */
    void setTable(StoreTable* pOwner);
    /**
      Get pointer to filter assigned to member
      @return pointer to filter instance
    */
    StoreableFilterBase* getFilter();
    /**@internal
      Get archive data type
      @return one of Database::StoreType enum values
    */
    int getDatabaseType();
    /**@internal
      Get next member from store table member list
      @return pointer to next member or NULL if not found
    */
    StoreTableMember* getNextMember() const;
    /**@internal
      Set next member - for internal use only
      @param pMember new value of mNextMember field
    */
    void setNextMember(StoreTableMember* pMember);    
    /**
      Deletes mFilter instance if mFilterOwner is true
    */
    ~StoreTableMember();
  private:
    StoreableFilterBase* mFilter;
    bool mFilterOwner;
    int mDatabaseType;
    StoreTableMember* mNextMember;
};

/**@internal
  store table definition
*/
class dbaDLLEXPORT StoreTable : public MemberTableBase<StoreTable,StoreTableMember> {
  public:
    StoreTable(const char* pTableName) : MemberTableBase<StoreTable,StoreTableMember>(pTableName) {};
    ~StoreTable() {};
  private:
};

class CollectionFilterBase;
class Storeable;

/**@internal 
  One entry in ColTable
*/
class dbaDLLEXPORT ColMemberEntry : public MemberEntryBase {
  public:
    ColMemberEntry(ColTable* pOwner, const char* pMemberName, int pMemberOffset, CollectionFilterBase* pFilter, const char* pFKeyName);
    ColMemberEntry(ColTable* pOwner, const char* pMemberName, int pMemberOffset, CollectionFilterBase* pFilter, const char* pFKeyName, const char* pTableName, id pRelationName);
    const char* getTableName() const;
    const char* getFKeyName() const;
    id getRelationId() const;
    ColMemberEntry* getNextMember() const;
    void setNextMember(ColMemberEntry* pNextMember);
    CollectionFilterBase* getFilter();
    ~ColMemberEntry();
  private:
    const char* mTableName;
    const char* mFKeyName;
    id mRelationId;
    CollectionFilterBase* mFilter;
    ColMemberEntry* mNextMember;
};

/**@internal 
  Static collection table for BIND_COL and BIND_CLA members
*/
class dbaDLLEXPORT ColTable : public MemberTableBase<ColTable,ColMemberEntry> {
  public:
    ColTable(const char* pName) : MemberTableBase<ColTable,ColMemberEntry>(pName) {};
    ~ColTable() {};
};

/**@internal 
  Static store table list
*/
class dbaDLLEXPORT StoreTableList {
  public:
    StoreTableList();
    std::set<StoreTable*> mStoreTables;
    std::set<ColTable*> mColTables;
    ~StoreTableList();
};

/**
  This macro declares store table inside Storeable derived class. Store table is used 
  to define what members of class should be serialized and how.
  @ingroup store_table
*/
#define DECLARE_STORE_TABLE() public: \
    virtual const dba::StoreTable* st_getTable(); \
    virtual const dba::ColTable* st_getColTable(); \
      protected: \
    static dba::StoreTable* st_table;\
    static dba::ColTable* st_ColTable;\
      private:\
    void st_createTables();  
/**
  This macro defines begin of store table definition.
  @param Class name of class that owns this table
  @param parent name of parent storeable class
  @param table name of relation where objects will be serialized (could be NULL)
  
  If table is NULL then name of relation of parent store table is used. 
  If last table in hierarchy has NULL relation name you have to provide relation
  name in open() method of stream or serialize using BIND_STB(member).
  @ingroup store_table
*/
#define BEGIN_STORE_TABLE(Class,parent,table) \
dba::StoreTable* Class::st_table = NULL;                  \
dba::ColTable* Class::st_ColTable = NULL;                  \
const dba::StoreTable*                                    \
Class::st_getTable() { \
  if (Class::st_table == NULL) \
    st_createTables();                          \
  return Class::st_table; \
};\
const dba::ColTable*                                    \
Class::st_getColTable() { \
  if (Class::st_ColTable == NULL) \
    st_createTables();                          \
  return Class::st_ColTable; \
}; \
void \
Class::st_createTables() {                            \
  st_table = new dba::StoreTable(table);                         \
  st_table->setClassOffset( (char*)(Class*)dba::Storeable::dba_pointer_place - (char*)dba_pointer_place ); \
  st_table->setNextTable(parent::st_getTable()); \
  sStoreTableList.mStoreTables.insert(st_table); \
  st_ColTable = new dba::ColTable(table);                         \
  st_ColTable->setClassOffset( (char*)(Class*)dba::Storeable::dba_pointer_place - (char*)dba_pointer_place ); \
  st_ColTable->setNextTable(parent::st_getColTable()); \
  sStoreTableList.mColTables.insert(st_ColTable);            

/**
  This macro defines class member binded to relation field that shoud be converted to text in archive.
  @param member name of member. Must be prefixed with class name.
  @param filter_class name of dba::StoreableFilter derived class that should peform conversion
  @param field name of column in relation where member should be stored
  @ingroup store_table
*/
#define BIND_STR(member,filter_class,field) \
  new dba::StoreTableMember(st_table,field,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),dba::Database::STRING);

/**
  This macro defines class member binded to relation field that shoud be converted to numeric value in archive.
  @param member name of member. Must be prefixed with class name.
  @param filter_class name of dba::StoreableFilter derived class that should peform conversion
  @param field name of column in relation where member should be stored
  @ingroup store_table
*/
#define BIND_INT(member,filter_class,field) \
  new dba::StoreTableMember(st_table,field,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),dba::Database::INTEGER);

/**
  This macro defines class member binded to relation field that shoud be converted to real value in archive.
  @param member name of member. Must be prefixed with class name.
  @param filter_class name of dba::StoreableFilter derived class that should peform conversion
  @param field name of column in relation where member should be stored
  @ingroup store_table
*/
#define BIND_FLT(member,filter_class,field) \
  new dba::StoreTableMember(st_table,field,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),dba::Database::FLOAT);

/**
  This macro defines class member binded to relation field that shoud be converted to date or time value in archive.
  @param member name of member. Must be prefixed with class name.
  @param filter_class name of dba::StoreableFilter derived class that should peform conversion
  @param field name of column in relation where member should be stored
  @ingroup store_table
*/
#define BIND_DAT(member,filter_class,field) \
  new dba::StoreTableMember(st_table,field,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),dba::Database::DATE);

/**
  Macro for storing list of related objects.
  @warning if you need to add BIND_COL for two or more lists of objects of the same type you have to use BIND_CLA(member,filter_class,fkname,coll_id,table_name) instead of this macro
  @param member member that holds list of storeable derived objects.
  @param filter_class name of dba::CollectionFilter that will perform iteration on member
  @param fkname name of column that holds foreign key to parent object or name of xml element that is a parent for this list
  @ingroup store_table
*/
#define BIND_COL(member,filter_class,fkname) \
  new dba::ColMemberEntry(st_ColTable,NULL,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),fkname);

/**
  Macro for storing subobjects from various collections using custom root table name and dba_coll_id field.
  This macro must be used instead of BIND_COL(member,filter_class,fkname), when:
  - objects in member list should be stored in different relation that specified in object store table
  - there are two or more members that holds objects of the same type.
  @param member member that holds list of storeable derived objects.
  @param filter_class name of dba::CollectionFilter that will perform iteration on member
  @param fkname name of column that holds foreign key to parent object or name of xml element that is a parent for this list
  @param coll_id numeric value that identifies collection - may be NULL
  @param table_name name of relation where objects from member collection should be stored.
  @ingroup store_table
*/
#define BIND_CLA(member,filter_class,fkname,coll_id,table_name) \
  new dba::ColMemberEntry(st_ColTable,NULL,&reinterpret_cast<char &>(member) - (char*)this,new filter_class(member),fkname,table_name,coll_id);

/**
  @ingroup store_table
  This macro transfers all members binded from one table to another one.
  Detailed.
*/
#define BIND_STB(member) \
  {  \
    const dba::StoreTable* table = member.st_getTable(); \
    while(table != NULL) {      \
      const dba::StoreTableMember* entry = table->getMembers(); \
      while(entry != NULL) { \
        dba::StoreTableMember* tmp = new dba::StoreTableMember(*entry);                   \
        tmp->setFilterOwner(false); \
        tmp->setMemberOffset(tmp->getMemberOffset() + ((char*)&(member) - (char*)this)); \
        st_table->addMember(tmp); \
        entry = entry->getNextMember();           \
      };                                \
      table = table->getNextTable();         \
    }; \
  };
/**
  This macro defines end of store table
  @ingroup store_table
*/
#define END_STORE_TABLE()   \
};

class Archive;

/** 
Class that defines objects storeable in Archive using obiject ouytput streams and loadable from
Archive using object input streams.
Derived classes must provide default constructor and copy constructor.
@ingroup api
*/
class dbaDLLEXPORT Storeable {
    friend class Stream;
  public:
    /**
      Helper class for locking Storeable data when doing Archive operations. Use it instead of Storeable::lockStid and Storeable::unlockStid. This class should be created on stack.
      @deprecated use IdLocker instead
      Example of use:
      @code
      Storable a; //new object
      Storeable b = loadFromDatabase(); //object with id
      
      Storeable::StidLocker locker(b);
      b = a; // b still has its id
      @endcode
    */
    class StidLocker {
      public:
        /**
          Constructor
          @param pObj object to lock
          @param pPreserveState if false object becomes changed, else object will preserve old state
        */
        StidLocker(Storeable& pObj, bool pPreserveState = false) 
          : mObj(pObj)
        {
          mObj.lockStid(pPreserveState);
        };
        /**
          Destructor
        */
        ~StidLocker() {
          mObj.unlockStid();
        };
      private:
        StidLocker(const StidLocker&);
        StidLocker& operator=(const StidLocker&);
        Storeable& mObj;
    };
    /**
      Generic optimal way to change member of persistant object
    */
    template <typename T> bool setMember(T& pMember, const T& pNevVal);
    /**
       id of object that was now stored in database.<br>
       If object has this id it means that id was never assigned
    */
    static const id InvalidId = 0;
    /**
      Internal pointer for computing offsets corrections in case of multiple inheritance
    */
    static const Storeable* const dba_pointer_place;

    /**
      Static list of created tables
      Responsible for freeing all memory allocated by BEGIN_STORE_TABLE macro
    */
    static StoreTableList sStoreTableList;

    //!state of storeable object
    typedef enum {
      //! object is newly created. Sync() will insert it into database
      NEW,
      //! object was changed since last sync()
      CHANGED,
      //! object is deleted. sync() will delete it from database
      DELETED,
      //! object is not changed since last sync()
      OK
    } stState;
    /**
      Create new object with id
      @param pId id of object or InvalidId if unknown
    */
    Storeable(id pId = InvalidId);
    /**
      Copy constructor. Copies object state if no IdLocker is constructed.
      @param pObj object to copy from
    */
    Storeable(const Storeable& pObj);
    /**
      Assignent operator. Copies object state if no IdLocker is constructed.
      @param pObj object to copy from
    */
    Storeable& operator=(const Storeable& pObj);
    /**
      assingn id by hand. Allowed only if getId() == InvalidId
      @warning Do not use this method unless you really know what you are doing. If you want clone Storeable object use IdLocker instead.
      @param pId new id
    */
    void setId(id pId);
    /**
      set object state to CHANGED 
      @see stState
    */
    void setChanged();
    /**
      get name of root table
      @return pointer to root table name or NULL if root table name is not set
    */
    const char* getRootTable();
    /**
      set object to NEW state, reset Id and mStoredTables
    */
    virtual void setNew();
    /**
      set object state to DELETED 
      @see stState
    */
    void setDeleted();
    /**
      get object state 
      @see stState
    */
    stState getState() const;
    /**
      set object state to OK 
      @see stState
    */
    void setOk();
    /**
      Lock id. Used by StidLocker to lock object id before assigment.
      @param pPreserveState if true then lock state but not id, otherwise lock both state and id
    */
    virtual void lockStid(bool pPreserveState);
    /**
      Unlock state and id of object for copy constructor and assigment operator
    */
    virtual void unlockStid();
    /**
      Set state of object.
      @param pState new object state.
      Result state after calling of this function is dependent of
      current object state (mState) and pState parameter.

      <table>
      <tr>
        <td><b>new state<br>old state</b></td>
        <td><b>OK</b></td>
        <td><b>NEW</b></td>
        <td><b>CHANGED</b></td>
        <td><b>DELETED</b></td>
      </tr>
      <tr>
        <td><b>OK</b></td>
        <td>OK</td>
        <td>CHANGED</td>
        <td>CHANGED</td>
        <td>DELETED</td>
      </tr>
      <tr>
        <td><b>NEW</b></td>
        <td>NEW</td>
        <td>NEW</td>
        <td>NEW</td>
        <td>NEW</td>
      </tr>
      <tr>
        <td><b>CHANGED</b></td>
        <td>OK</td>
        <td>CHANGED</td>
        <td>CHANGED</td>
        <td>DELETED</td>
      </tr>
      <tr>
        <td><b>DELETED</b></td>
        <td>OK</td>
        <td>CHANGED</td>
        <td>CHANGED</td>
        <td>DELETED</td>
      </tr>
      </table>
    */
    virtual void setState(stState pState);
    /**
      get object id
      @returns object id
    */
    id getId() const;
    /**
      Check if object is in DELETED state
      @return true if object is in DELETED state or false otherwise
    */
    bool isDeleted() const;
    /**
      Check if object is in CHANGED state
      @return true if object is in CHANGED state or false otherwise
    */
    bool isChanged() const;
    /**
      Check if object is in NEW state
      @return true if object is in NEW state or false otherwise
    */
    bool isNew() const;
    /**
      Check if object is in OK state
      @return true if object is in OK state or false otherwise
    */
    bool isOk() const;
    /**
      Destructor
    */
    virtual ~Storeable();
  protected:  
    /**
      state of lock on object data for assigment operator and copy constructor
    */
    typedef enum {
      UNLOCKED = 0, //!state and id is copied
      LOCKED = 1, //!state and id is preserved
      LOCKED_PRESERVE_STATE = 2 //! state is preserved, id is copied
    } lock_state;
    /**
      Get store table list for object 
    */
    virtual const StoreTable* st_getTable();
    /**
      Get collection table list for object      
    */
    virtual const ColTable* st_getColTable();
    /**
      Variant of assigment operator. Calls setChanged()
      @param pObj not used
    */
    void preserveIdAssigment(const Storeable& pObj);
    /**
      Variant of assigment operator. Copies state.
      @param pObj object to copy from      
    */
    void normalAssigment(const Storeable& pObj);
    /**
      id of object 
    */
    id mId;
    /**
      number of tables from first one that are already stored for object 
    */
    int mStoredTables;
    /**
      Get number  
    */
    int countTables();
    /**
      Store state
    */
    stState mStoreState;
    /**
      Lock state
    */
    lock_state mIdLocked;
};

/**
  helper function to set member of Storeable derived object. It changes state of object to CHANGED if new value is different from old value
  @param pMember member value
  @param pNewVal new value
  @returns true if member was set or false otherwise
*/
template <typename T> 
bool
Storeable::setMember(T& pMember, const T& pNewVal) {
  if (pMember == pNewVal)
    return false;
  pMember = pNewVal;
  setChanged();
  return true;
};

//==================== MemberTableBase ==================
template <typename T, typename M>
MemberTableBase<T,M>::MemberTableBase(const char* pTableName)
  : mTableName(pTableName),
    mClassOffset(0),
    mNextTable(NULL),
    mMembers(NULL)
{};

template <typename T, typename M>
const char* 
MemberTableBase<T,M>::getTableName() const {
  return mTableName;
};

template <typename T, typename M>
int 
MemberTableBase<T,M>::getClassOffset() const {
  return mClassOffset;
};

template <typename T, typename M>
void 
MemberTableBase<T,M>::setClassOffset(int pOffset) {
  mClassOffset = pOffset;
};

template <typename T, typename M>
const T* 
MemberTableBase<T,M>::getNextTable() const {
  return mNextTable;
};

template <typename T, typename M>
M* 
MemberTableBase<T,M>::getMembers() const {
  return mMembers;
};

template <typename T, typename M>
void 
MemberTableBase<T,M>::setNextTable(const T* pTable) {
  mNextTable = pTable;
};

template <typename T, typename M>
void 
MemberTableBase<T,M>::addMember(M* pMember) {
  pMember->setNextMember(mMembers);
  mMembers = pMember;
};

template <typename T, typename M>
MemberTableBase<T,M>::~MemberTableBase() {
  while (mMembers != NULL) {
    M* next = mMembers->getNextMember();
    delete mMembers;
    mMembers = next;
  };
};

};//namespace


#endif
