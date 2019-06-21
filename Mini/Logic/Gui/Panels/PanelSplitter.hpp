//
//  PanelSplitter.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 14/04/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"
#include "PanelLocation.hpp"

namespace Mini {
    using namespace ECS;
    struct PanelSplitter {
        GameObject area;
        PanelLocation location;
        bool isHorizontal;
    };
}
