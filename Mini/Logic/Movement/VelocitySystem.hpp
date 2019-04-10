//
//  SheetMovementSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "System.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"

namespace Mini {
    using namespace ECS;
    struct VelocitySystem : System<Transform, Velocity> {
    public:
        void Update(float dt);
    };
}
