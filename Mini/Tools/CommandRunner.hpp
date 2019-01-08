//
//  ProcessRunner.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/12/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>

namespace Mini {
    class CommandRunner {
    public:
        static std::vector<std::string> Run(const std::string& cmd);
    };
}
