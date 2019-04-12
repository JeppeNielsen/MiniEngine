//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"
#include "OctreeSystem.hpp"
#include "CameraSystem.hpp"
#include "ObjectRenderer.hpp"
#include "ShaderCollection.hpp"

namespace Mini {
    using namespace ECS;
    struct RenderSystem : System<Transform, Mesh, Renderable> {
    public:
        using OctreeSystem = OctreeSystem<Renderable>;
        
        using ObjectRenderers = std::vector<IObjectRenderer*>;
        using VisibleObjects = std::vector<VisibleObject>;

        RenderSystem(OctreeSystem& octree, CameraSystem& cameras);
        ~RenderSystem();
        
        OctreeSystem& Octree();
        
        void Render();
        
        ShaderCollection Shaders;
        IShader* DefaultShader;
        IShader* DefaultTexturedShader;
        
    private:
        
        static ObjectRenderers objectRenderers;
        static int objectRenderersRefCounter;
        
        VisibleObjects opaqueObjects;
        VisibleObjects transparentObjects;

        RenderInfo renderInfo;
        ObjectCollection objectsInFrustum;

        OctreeSystem& octree;
        CameraSystem& cameras;
        
        void RenderCamera(GameObject cameraObject);
        void RenderVisibleObjects(const VisibleObjects& visibleObjects);
        void RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects);
        
        static bool SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b);
        static bool SortTransparentObjects(const VisibleObject& a, const VisibleObject& b);
    };
}
