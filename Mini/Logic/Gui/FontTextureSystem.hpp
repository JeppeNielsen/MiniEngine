//
//  FontTextureSystem.hpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "TextureComponent.hpp"
#include "Font.hpp"

namespace Mini {
    using namespace ECS;
    struct FontTextureSystem : System<Font, TextureComponent> {
    protected:
        void Update(float dt);
    };
}
