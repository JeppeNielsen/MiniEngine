//
//  Window.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 04/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include "InputDevice.hpp"

namespace Mini {
    class Window {
    public:
       using OnInitialize = std::function<void()>;
       using OnUpdate = std::function<bool()>;
       using OnScreenSizeChanged = std::function<void(int, int)>;
    
       void StartLoop(OnInitialize onInitialize,
                      OnUpdate onUpdate,
                      OnScreenSizeChanged onScreenSizeChanged);
        
       void PreRender();
       void PostRender();
        
        OnInitialize onInitialize;
        OnUpdate onUpdate;
        OnScreenSizeChanged onScreenSizeChanged;
        InputDevice inputDevice;
    };
}
