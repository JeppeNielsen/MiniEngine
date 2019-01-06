//
//  Device.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "InputManager.hpp"

namespace Mini {
    class Engine;
    class Device {
    public:
        void Exit();
        InputManager Input;
        Property<Vector2> ScreenSize;
    private:
        bool exited = false;
        friend class Engine;
    };
}
