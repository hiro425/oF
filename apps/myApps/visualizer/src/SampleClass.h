//
//  SampleClass.h
//  TestCatch
//
//  Created by Hilton Lipschitz on 2014-10-09.
//  Copyright (c) 2014 Hilton Lipschitz. All rights reserved.
//

#ifndef __TestCatch__SampleClass__
#define __TestCatch__SampleClass__

//#include <stdio.h>
#include <string>

class SampleClass {
 
public:
  std::string getStr() { return _str; };
  void setStr(const std::string& s) { _str = s; };
  
  double getD() { return _d; };
  void setD(double x) { _d = x; };
  
  std::string bigStr();
  
private:
  std::string _str;
  double _d;
  
};

#endif /* defined(__TestCatch__SampleClass__) */
