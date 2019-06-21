//
//  CoreECS.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestHierarchy.hpp"
#include "ECS.hpp"

using namespace Mini::ECS;
using namespace Tests::ECS;

void TestHierarchy::Run() {

    RunTest("Hierarchy component added by default", [] () -> bool {
        Database database;
        Scene scene(database);
        auto object = scene.CreateObject();
        return object.GetComponent<Hierarchy>() != nullptr;
    });
    
    RunTest("Hierarchy component default parent is null", [] () -> bool {
        Database database;
        Scene scene(database);
        auto object = scene.CreateObject();
        return object.GetComponent<Hierarchy>()->Parent() == nullptr;
    });
    
    RunTest("Children count 2", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child1 = scene.CreateObject();
        child1.Hierarchy().Parent = parent;
        
        auto child2 = scene.CreateObject();
        child2.Hierarchy().Parent = parent;
        
        return parent.Hierarchy().Children().size() == 2;
    });
    
    RunTest("Parent cannot be set to it self", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child1 = scene.CreateObject();
        child1.Hierarchy().Parent = parent;
        
        child1.Hierarchy().Parent = child1;
        
        return child1.Hierarchy().Parent() == parent;
    });
    
    RunTest("Parent cannot be set to child", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child1 = scene.CreateObject();
        child1.Hierarchy().Parent = parent;
        
        parent.Hierarchy().Parent = child1;
        
        return parent.Hierarchy().Parent() == nullptr;
    });
    
    
    RunTest("Parent unchanged on setting on invalid parent", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto top = scene.CreateObject();
        
        auto middle = scene.CreateObject();
        middle.Hierarchy().Parent = top;
        
        auto bottom = scene.CreateObject();
        bottom.Hierarchy().Parent = middle;
        
        middle.Hierarchy().Parent = bottom;
        
        return middle.Hierarchy().Parent() == top;
    });
    
    RunTest("Children count reduced when child reparented", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child1 = scene.CreateObject();
        child1.Hierarchy().Parent = parent;
        
        auto child2 = scene.CreateObject();
        child2.Hierarchy().Parent = parent;
        
        bool wasTwo = parent.Hierarchy().Children().size() == 2;
    
        child2.Hierarchy().Parent = nullptr;
        
        bool isOne = parent.Hierarchy().Children().size() == 1;
    
        return wasTwo && isOne;
    });
    
    RunTest("Child also deleted when parent is deleted", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        
        parent.Remove();
        
        scene.Update(0);
        
        return !child;
    });
    
    RunTest("Child part of parent's children list", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        
        auto& children = parent.Hierarchy().Children();
        
        return std::find(children.begin(), children.end(), child) != children.end();
    });
    
    RunTest("Child removed from parent's children list on remove", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        
        auto& children = parent.Hierarchy().Children();
        
        bool wasInList = std::find(children.begin(), children.end(), child) != children.end();
    
        child.Remove();
        
        scene.Update(0);
    
        bool removedFromList = std::find(children.begin(), children.end(), child) == children.end();
    
        return wasInList && removedFromList;
    });
    
    
    RunTest("Child removed from parent's children list on reparent", [] () -> bool {
        Database database;
        Scene scene(database);
        
        auto parent = scene.CreateObject();
        
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        
        auto& children = parent.Hierarchy().Children();
        
        bool wasInList = std::find(children.begin(), children.end(), child) != children.end();
    
        child.Remove();
        
        scene.Update(0);
    
        bool removedFromList = std::find(children.begin(), children.end(), child) == children.end();
    
        return wasInList && removedFromList;
    });
}

