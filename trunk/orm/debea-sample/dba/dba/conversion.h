// File: conversion.h
// Purpose: Various converison utilities
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2001
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)


#ifndef DBACONVERSION_H
#define DBACONVERSION_H

#include <string>
#include <typeinfo>
#include <iostream>

#include "dba/defs.h"
#include "dba/exception.h"

extern "C" {
#include <errno.h>
}

namespace dba {

/**conversion between string and base numeric types
  */

/**
  Exception thrown by data types conversion routines.
*/
class dbaDLLEXPORT ConversionException: public Exception {
  public:
    ConversionException(const std::string& mesg) : Exception(mesg.c_str()) {};
};

/**
  replace all characters in s from to_find to repl_with
*/
dbaDLLEXPORT int replaceAll(std::string &s, const std::string &to_find, const std::string& repl_with);


/**
  trim empty chars from begin and end of string
*/
dbaDLLEXPORT void trim(std::string& s);

dbaDLLEXPORT struct tm
parseISODateTime(const char* pDate);

dbaDLLEXPORT struct tm
parseISODate(const char* pDate);

/*removes isspace() chars from end and beginning of string*/
dbaDLLEXPORT std::string
strip(const std::string& s);

/*replaces all ' by ''*/
dbaDLLEXPORT std::string
escape_sql(const std::string& s);

/*in-place conversions*/
dbaDLLEXPORT void
convert(const std::string& s,long& i, int base = 0);

dbaDLLEXPORT void
convert(const std::string& s,int& i, int base = 0);

dbaDLLEXPORT void
convert(const std::string& s,double& d, const char* pDecPointRepl = NULL);

dbaDLLEXPORT void
convert(const char *s,int& i, int base = 0);

dbaDLLEXPORT void
convert(const char *s,long& i, int base = 0);

dbaDLLEXPORT void
convert(const char* s, double& d, const char* pDecPointRepl = NULL);

dbaDLLEXPORT void
convert(int i, std::string& s);

dbaDLLEXPORT void
convert(long l, std::string& s);

dbaDLLEXPORT void
convert(double d, std::string& s);

dbaDLLEXPORT void
convert(int i, char* s);

dbaDLLEXPORT void
convert(long l, char* s);

dbaDLLEXPORT void
convert(double d, char* s);

/*basic datatypes to string*/
dbaDLLEXPORT std::string toStr(long i,const char* format = "%d");
dbaDLLEXPORT std::string toStr(unsigned i,const char* format = "%u");
dbaDLLEXPORT std::string toStr(int i,const char* format = "%d");
dbaDLLEXPORT std::string toStr(double d,const char* format = "%f");

//returns n-th word from string
dbaDLLEXPORT std::string getWord(const std::string& str,unsigned int num);

}//namespace

#endif

