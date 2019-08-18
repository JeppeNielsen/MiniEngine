//
//  TestECS.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestBase.hpp"
#include "ScriptWorld.hpp"

namespace Tests {
namespace ECS {
    class TestScripting : public TestBase {
    public:
        void Run();
    private:
        void SetupScriptWorld(Mini::ScriptWorld& scriptWorld);
    };
}
}
