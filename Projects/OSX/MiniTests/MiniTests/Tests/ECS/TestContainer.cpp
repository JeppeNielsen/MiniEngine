//
//  CoreECS.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestContainer.hpp"
#include "Container.hpp"

using namespace Mini::ECS;
using namespace Tests::ECS;

void TestContainer::Run() {

    struct Position {
        int x = 123;
        int y = 456;
    };

    RunTest("Container::CreateDefault", [] () -> bool {
    
        int hej = 1;
        int test1 = --hej;
        
        int hej2 = 1;
        int test2 = hej2--;
    
    
        Container<Position> container;
        container.CreateDefault(0);
        return container.elements.size() == 1;
    });
    
    RunTest("Container::Get", [] () -> bool {
        Container<Position> container;
        container.CreateDefault(0);
        return container.Get(0)->x == 123;
    });
    
    RunTest("Container::Destroy", [] () -> bool {
        Container<Position> container;
        container.CreateDefault(0);
        container.Destroy(0);
        return container.elements.size() == 0;
    });
    
    RunTest("Container::Destroy 2nd element", [] () -> bool {
        Container<Position> container;
        container.CreateDefault(0);
        container.CreateDefault(1);
        
        container.Destroy(0);
        container.Destroy(1);
        
        return container.elements.size() == 0;
    });
    
    RunTest("Container::Destroy twice", [] () -> bool {
        Container<Position> container;
        container.CreateDefault(0);
        container.CreateDefault(1);
        container.CreateDefault(2);
        
        container.Destroy(0);
        container.Destroy(1);
        
        return container.Get(2)->x == 123;
    });
}
