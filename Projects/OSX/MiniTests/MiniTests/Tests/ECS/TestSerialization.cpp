//
//  TestSerialization.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 28/06/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "TestSerialization.hpp"
#include "JsonSerializer.hpp"
#include "ECS.hpp"

using namespace Mini;
using namespace Tests::ECS;
using namespace Mini::ECS;

#define TARGET_STRING(...) #__VA_ARGS__

void TestSerialization::Run() {
    
    RunTest("Serialize plain GameObject",[]() {
        
        Database database;
        Scene scene(database);
        auto object = scene.CreateObject();
        
        JsonSerializer serializer;
        std::stringstream ss;
        serializer.SerializeObject(object, ss);
        std::string target = "{\n    \"GameObject\": {\n        \"Components\": {}\n    }\n}";
        return ss.str() == target;
    });

}
