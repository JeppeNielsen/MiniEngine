//
//  SpriteTextureSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Sprite.hpp"
#include "Atlas.hpp"

namespace Mini {
    struct SpriteTextureSystem : System<Sprite, Atlas, Mesh> {
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void SpriteChanged(GameObject object);
    };
}
