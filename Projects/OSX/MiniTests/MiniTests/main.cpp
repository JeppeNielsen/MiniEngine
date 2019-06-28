//
//  main.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 12/06/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "TestCore.hpp"
#include "TestHierarchy.hpp"
#include "TestEnabling.hpp"
#include "TestReflection.hpp"

using namespace Tests;

int main() {

    ECS::TestCore ecsCore;
    ecsCore.Run();
    std::cout << std::endl;
    ECS::TestHierarchy ecsHierarchy;
    ecsHierarchy.Run();
    std::cout << std::endl;
    ECS::TestEnabling testEnabling;
    testEnabling.Run();
    
    std::cout << std::endl;
    ECS::TestReflection testReflection;
    testReflection.Run();
    
    return 0;
}
