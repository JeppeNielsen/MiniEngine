//
//  LimitableSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Scene.hpp"
#include "Transform.hpp"
#include "Limitable.hpp"

namespace Mini {
    using namespace ECS;
    class LimitableSystem : System<Transform, Limitable> {
    public:
        void Update(float dt);
    };
}
