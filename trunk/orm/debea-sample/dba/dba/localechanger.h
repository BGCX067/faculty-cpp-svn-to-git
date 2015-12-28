// File: localechanger.h
// Purpose: Various converison utilities
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2001
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)

#ifndef DBALOCALECHANGER_H
#define DBALOCALECHANGER_H

#include "dba/defs.h"

namespace dba {

/**
Class for temporarly altering C locale with new value
*/
class dbaDLLEXPORT LocaleChanger {
  public:
    LocaleChanger(int pCategory, const char* pNewLocale);
    bool isOk() const;
    ~LocaleChanger();
  private:
    char* mSavedLocale;
    int mCategory;
};

} //namespace

#endif
