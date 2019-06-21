//
//  Test.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestBase.hpp"
#include <iostream>

using namespace Tests;

void TestBase::RunTest(const std::string& testName, const std::function<bool ()>& testCode) const {
    bool result = testCode();
    std::cout << testName << ": " << (result ? "SUCCES" : "FAILED") << std::endl;
}
