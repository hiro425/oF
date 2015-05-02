//
//  testSampleClass.cpp
//  TestCatch
//
//  Created by Hilton Lipschitz on 2014-10-09.
//  Copyright (c) 2014 Hilton Lipschitz. All rights reserved.
//

#include "catch.hpp"

#include "SampleClass.h"

TEST_CASE("Testing Sample Class") {
  SampleClass sc;
  
  SECTION("setting the str") {
    INFO("Using TestStr") // Only appears on a FAIL
    sc.setStr("TestStr");
    CAPTURE(sc.getStr()); // Displays this variable on a FAIL
    
    CHECK(sc.getStr() == "TestStr");
  }
  
  SECTION("test bigStr") {
    sc.setStr("TestStr");
    REQUIRE(sc.bigStr() == "TestStr : 7");
  }
  
  SECTION("Test doubles") {
    sc.setD(1);
    CHECK(sc.getD() == 1);
    sc.setD(1.0/3.0);
    CHECK(sc.getD() == Approx(0.33333)); // Nice
  }
}
