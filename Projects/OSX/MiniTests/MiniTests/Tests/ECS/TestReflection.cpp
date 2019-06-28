//
//  TestReflection.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 28/06/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "TestReflection.hpp"
#include "TypeInfo.hpp"

using namespace Mini;
using namespace Tests::ECS;

void TestReflection::Run() {
    
    RunTest("Field exists",[]() {
    
        struct Transform {
            int position;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(position)
            TYPE_FIELDS_END
        };
        
        Transform transform { 123 };
        auto typeInfo = transform.GetType();
        
        FieldInfo info;
        return typeInfo.TryFindField("position",info);
    });

    RunTest("FieldInfo value == 123",[]() {
    
        struct Transform {
            int position;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(position)
            TYPE_FIELDS_END
        };
    
        Transform transform { 123 };
        auto typeInfo = transform.GetType();
    
        FieldInfo info;
        typeInfo.TryFindField("position",info);
        return *info.GetField<int>() == 123;
    });

    RunTest("3 FieldInfos",[]() {
    
        struct Transform {
            int x;
            int y;
            int z;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(x)
            TYPE_FIELD(y)
            TYPE_FIELD(z)
            TYPE_FIELDS_END
        };
    
        Transform transform { 2,3,4 };
        auto typeInfo = transform.GetType();
        return typeInfo.Fields().size()==3;
    });

    RunTest("2 field names",[]() -> bool {
    
        struct Pair {
            std::string first;
            std::string second;
            
            TYPE_FIELDS_BEGIN
            TYPE_FIELD(first)
            TYPE_FIELD(second)
            TYPE_FIELDS_END
        };
    
        Pair pair {"one", "two"};
        
        auto typeInfo = pair.GetType();
        return
        typeInfo.Fields()[0].Name()=="first" &&
        typeInfo.Fields()[1].Name()=="second";
    });


}
