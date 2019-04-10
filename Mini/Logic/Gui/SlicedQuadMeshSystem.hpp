//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "SlicedTexture.hpp"
#include "SlicedQuad.hpp"

namespace Mini {
    using namespace ECS;
    struct SlicedQuadMeshSystem : System<SlicedQuad, SlicedTexture, Sizeable, Mesh> {
    public:
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void UpdateMesh(GameObject object);
    };
}
