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
#include "TestSerialization.hpp"
#include "TestAssetDatabase.hpp"
#include "TestObjectList.hpp"
#include "TestScripting.hpp"

using namespace Tests;

int main() {

    std::cout << std::endl << "Core" << std::endl << std::endl;
    ECS::TestCore ecsCore;
    ecsCore.Run();
    
    std::cout << std::endl << "Hierarchy" << std::endl << std::endl;
    ECS::TestHierarchy ecsHierarchy;
    ecsHierarchy.Run();
    
    std::cout << std::endl << "Enabling" << std::endl << std::endl;
    ECS::TestEnabling testEnabling;
    testEnabling.Run();
    
    std::cout << std::endl << "Reflection" << std::endl << std::endl;
    ECS::TestReflection testReflection;
    testReflection.Run();
    
    std::cout << std::endl << "Serialization" << std::endl << std::endl;
    ECS::TestSerialization testSerialization;
    testSerialization.Run();
    
    std::cout << std::endl << "AssetDatabase" << std::endl << std::endl;
    ECS::TestAssetDatabase testAssetDatabase;
    testAssetDatabase.Run();
    
    std::cout << std::endl << "ObjectList" << std::endl << std::endl;
    ECS::TestObjectList testObjectList;
    testObjectList.Run();
    
    std::cout << std::endl << "Scripting" << std::endl << std::endl;
    ECS::TestScripting testScripting;
    testScripting.Run();
    
    return 0;
}
