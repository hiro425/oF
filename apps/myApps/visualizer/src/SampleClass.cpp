//
//  SampleClass.cpp
//  TestCatch
//
//  Created by Hilton Lipschitz on 2014-10-09.
//  Copyright (c) 2014 Hilton Lipschitz. All rights reserved.
//

#include "SampleClass.h"

#include <sstream>

std::string SampleClass::bigStr()
{
  std::stringstream ss;
  ss << _str << " : " << _str.length();
  return ss.str();
}