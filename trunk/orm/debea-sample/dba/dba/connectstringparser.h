// File: connectstringparser.h
// Purpose: Parsing connect string parameters
// Author: Lukasz Michalski <lm at zork.pl>, Copyright 2007
// License: See COPYING file that comes with this distribution
//
// This file is a part of debea library (http://debea.net)
//
//

#ifndef DBACONNECSTRINGPARSER_H
#define DBACONNECSTRINGPARSER_H

#include "dba/exception.h"
#include <vector>
#include <map>
#include <string>

namespace dba {

/**
  Parser for dba connect strings
*/
class dbaDLLEXPORT ConnectStringParser {
  public:
    ConnectStringParser(const char* pParams) : mParams(pParams) {}
    std::map<std::string,std::string> parse();
  private:
    typedef enum {
      SCAN,
      PARAM_NAME,
      QUOTED_NAME,
      PARAM_VALUE,
      QUOTED_VALUE,
      QUOTE_TEST
    } state;
    std::map<std::string, std::string> mResult;
    std::vector<state> mStack;
    std::string mName;
    std::string mToken;
    bool mGotAssignMark;
    const char* mEye;
    const char* mParams;

    void throwError(const char* pCause);
};

} //namespace

#endif
