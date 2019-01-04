//
//  Window.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 04/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>

namespace Mini {
    class Window {
    public:
       using OnInitialize = std::function<void()>;
       using OnUpdate = std::function<bool()>;
       
    
       void StartLoop(OnInitialize onInitialize,
                      OnUpdate onUpdate);
        
       void PreRender();
       void PostRender();
        
        OnInitialize onInitialize;
        OnUpdate onUpdate;
    };
}
