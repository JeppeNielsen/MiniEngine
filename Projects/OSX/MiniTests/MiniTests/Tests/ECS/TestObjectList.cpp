//
//  CoreECS.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestObjectList.hpp"
#include "ObjectList.hpp"

using namespace Mini::ECS;
using namespace Tests::ECS;

void TestObjectList::Run() {

    RunTest("ObjectList::Add", [] () -> bool {
        ObjectList list;
        list.Add(0);
        return list.objects.size() == 1;
    });
    
    RunTest("ObjectList::Contains", [] () -> bool {
        ObjectList list;
        list.Add(123);
        return list.Contains(123);
    });
    
    RunTest("ObjectList::Remove", [] () -> bool {
        ObjectList list;
        list.Add(123);
        list.Remove(123);
        return !list.Contains(123);
    });
    
    RunTest("ObjectList remove non last", [] () -> bool {
        ObjectList list;
        list.Add(1);
        list.Add(0);
        list.Remove(1);
        return std::find(list.objects.begin(), list.objects.end(), 1) == list.objects.end();
    });
}
