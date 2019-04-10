//
//  SpriteTextureSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Sprite.hpp"
#include "Atlas.hpp"

namespace Mini {
    using namespace ECS;
    struct SpriteTextureSystem : System<Sprite, Atlas, Mesh> {
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void SpriteChanged(GameObject object);
    };
}
