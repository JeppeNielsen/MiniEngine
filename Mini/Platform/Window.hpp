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
       struct MainLoopData {
           std::function<void()> Initialize;
           std::function<bool()> Update;
           std::function<void(int,int)> ScreenSize;
           std::function<void(float)> ScreenScalingFactor;
       };
    
       void StartLoop(MainLoopData mainLoopData);
        
       void PreRender();
       void PostRender();
        
       MainLoopData mainLoopData;
       InputDevice inputDevice;
    };
}
