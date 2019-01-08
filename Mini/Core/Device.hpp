//
//  Device.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Screen.hpp"
#include "InputManager.hpp"

namespace Mini {
    class Engine;
    class Device {
    public:
        void Exit();
        Screen Screen;
        InputManager Input;
    private:
        bool exited = false;
        friend class Engine;
    };
}
