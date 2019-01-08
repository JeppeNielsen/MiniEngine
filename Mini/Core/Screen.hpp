//
//  Screen.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 08/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Vector2.hpp"
#include "Property.hpp"

namespace Mini {
    class Screen {
    public:
        Property<Vector2> Size;
        Property<float> ScalingFactor;
        
        static Screen* MainScreen;
    };
}
