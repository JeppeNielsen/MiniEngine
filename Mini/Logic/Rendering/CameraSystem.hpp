//
//  CameraSystem.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 11/04/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

namespace Mini {
    using namespace ECS;
    struct CameraSystem : System<Transform, Camera> { };
}
