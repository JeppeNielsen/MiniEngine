//
//  Timer.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <chrono>

namespace Mini {
    class Timer {
    public:
        using Time = std::chrono::high_resolution_clock;
        void Start();
        float Stop();
    private:
        std::chrono::time_point<Time> startTime;
    };
}
