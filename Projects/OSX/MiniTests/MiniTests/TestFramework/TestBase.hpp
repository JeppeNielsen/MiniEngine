//
//  Test.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include <string>

namespace Tests {
    class TestBase {
    protected:
        void RunTest(const std::string& testName, const std::function<bool()>& testCode) const;
    public:
        virtual void Run() = 0;
    };
}
