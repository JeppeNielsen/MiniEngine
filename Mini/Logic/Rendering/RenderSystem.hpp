//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Scene.hpp"
#include "Screen.hpp"
#include "Camera.hpp"
#include "OctreeSystem.hpp"
#include "ObjectRenderer.hpp"
#include "ShaderCollection.hpp"

namespace Mini {
    using namespace ECS;
    struct RenderSystem : System<Transform, Mesh, Renderable> {
    public:
        using OctreeSystem = OctreeSystem<Renderable>;
        
        struct CameraSystem : System<Transform, Camera> { };
        struct TextureSystem : System<TextureComponent, Orderable> {};
        
        using ObjectRenderers = std::vector<IObjectRenderer*>;
        using VisibleObjects = std::vector<VisibleObject>;

        void Initialize() override;
        ~RenderSystem();
        
        OctreeSystem& Octree();
        
        void Render();
        
        ShaderCollection Shaders;
        IShader* DefaultShader;
        IShader* DefaultTexturedShader;
        
        void SetCameras(CameraSystem* cameraSystem);
        CameraSystem* GetCameras();
        CameraSystem* GetOriginalCameras();
        
    private:
        
        static ObjectRenderers objectRenderers;
        static int objectRenderersRefCounter;
        
        VisibleObjects opaqueObjects;
        VisibleObjects transparentObjects;

        RenderInfo renderInfo;
        ObjectCollection objectsInFrustum;

        CameraSystem* cameras;
        OctreeSystem* meshOctreeSystem;
        
        void RenderCamera(GameObject cameraObject);
        void RenderVisibleObjects(const VisibleObjects& visibleObjects);
        void RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects);
        
        static bool SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b);
        static bool SortTransparentObjects(const VisibleObject& a, const VisibleObject& b);
    };
}
