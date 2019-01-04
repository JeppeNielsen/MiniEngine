//
//  Timer.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Timer.hpp"

using namespace Mini;

void Timer::Start() {
    startTime = Time::now();
}

float Timer::Stop() {
    std::chrono::duration<float> elapsed = Time::now() - startTime;
    return elapsed.count();
}

