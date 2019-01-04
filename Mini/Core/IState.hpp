//
//  IState.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Device.hpp"

namespace Mini {
    class Engine;
    struct IState {
        virtual ~IState() = default;
        virtual void Initialize() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
    protected:
        Device device;
        friend class Engine;
    };
}
