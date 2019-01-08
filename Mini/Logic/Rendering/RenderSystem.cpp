//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "RenderSystem.hpp"
#include "Engine.hpp"
#include "ShaderComponent.hpp"

using namespace Mini;

RenderSystem::ObjectRenderers RenderSystem::objectRenderers;
int RenderSystem::objectRenderersRefCounter = 0;

void RenderSystem::Initialize() {
    cameras = &scene->CreateSystem<CameraSystem>();
    meshOctreeSystem = &scene->CreateSystem<OctreeSystem>();
    scene->CreateSystem<TextureSystem>();
    
    Shaders.Initialize();
    DefaultShader = &Shaders.Colored;
    DefaultTexturedShader = &Shaders.Textured;
    
    if (objectRenderersRefCounter == 0) {
        for (auto& vertexType : IVertexType::typelist) {
            objectRenderers.push_back(vertexType->CreateRenderer());
        }
    }
    objectRenderersRefCounter++;
}

RenderSystem::~RenderSystem() {
    objectRenderersRefCounter--;
    if (objectRenderersRefCounter == 0) {
        for (auto renderer : objectRenderers) {
            delete renderer;
        }
        objectRenderers.clear();
    }
}

RenderSystem::OctreeSystem& RenderSystem::Octree() {
    return *meshOctreeSystem;
}

void RenderSystem::RenderCamera(GameObject cameraObject) {

    const Vector2 screenSize = Screen::MainScreen->Size;;
    const float scalingFactor = Screen::MainScreen->ScalingFactor;
    const Vector2& scaledScreenSize = screenSize * scalingFactor;

    Transform* cameraTransform = cameraObject.GetComponent<Transform>();
    Camera* camera = cameraObject.GetComponent<Camera>();
    RenderMask cameraMask = camera->Mask;
    //if (!camera->Orthographic()) {
        const Rect& viewport = camera->Viewport;
        const Rect screenRect = viewport * scaledScreenSize;
        ASSERT_GL(glViewport(screenRect.x, screenRect.y, screenRect.width, screenRect.height));
    //}
    
    const Matrix4x4 viewProjection = camera->Projection().Multiply(cameraTransform->WorldInverse);
    
    BoundingFrustum frustum;
    frustum.SetFromViewProjection(viewProjection);
    meshOctreeSystem->GetObjectsInFrustum(frustum, objectsInFrustum);

    if (objectsInFrustum.empty()) return;
    
    const float* viewProjectionGL = viewProjection.GetGlMatrix();
    
    for (auto renderer : objectRenderers) {
        renderer->viewProjection = viewProjectionGL;
    }
    
    Vector3 distanceToCameraPosition;
    for(auto object : objectsInFrustum) {
        
        Renderable* renderable = object.GetComponent<Renderable>();
        RenderMask mask = renderable->Mask;
        if (!((cameraMask & mask) == mask)) { // must be matching camera mask
            continue;
        }
        
        Mesh* mesh = object.GetComponent<Mesh>();
        if (!mesh->vertexMesh) continue; // must have mesh
        
        Transform* transform = object.GetComponent<Transform>();
        
        const Matrix4x4& world = transform->World;
        distanceToCameraPosition.x = world[0][3];
        distanceToCameraPosition.y = world[1][3];
        distanceToCameraPosition.z = world[2][3];
        float fInvW = 1.0f / ( viewProjection[3][0] * distanceToCameraPosition.x + viewProjection[3][1] * distanceToCameraPosition.y + viewProjection[3][2] * distanceToCameraPosition.z + viewProjection[3][3] );
        float distanceToCamera = ( viewProjection[2][0] * distanceToCameraPosition.x + viewProjection[2][1] * distanceToCameraPosition.y + viewProjection[2][2] * distanceToCameraPosition.z + viewProjection[2][3] ) * fInvW;
        
        VisibleObjects& visibleObjects = renderable->BlendMode() == BlendModeType::Opaque ? opaqueObjects : transparentObjects;
        
        IShader* shader = 0;
        ShaderComponent* shaderComponent = object.GetComponent<ShaderComponent>();
        TextureComponent* textureComponent = object.GetComponent<TextureComponent>();
        if (shaderComponent) {
            shader = shaderComponent->GetBaseShader();
        }
        if (!shader) {
            shader = textureComponent ? DefaultTexturedShader : DefaultShader;
        }
        
        visibleObjects.push_back({
            transform,
            renderable,
            shader,
            mesh,
            mesh->VertexType(),
            textureComponent,
            object.GetComponent<Orderable>(),
            distanceToCamera
        });
    }
    objectsInFrustum.clear();
    
    ASSERT_GL(glEnable(GL_DEPTH_TEST));
    
    if (!opaqueObjects.empty()) {
        std::sort(opaqueObjects.begin(), opaqueObjects.end(), SortOpaqueObjects);
        ASSERT_GL(glDisable(GL_BLEND));
        ASSERT_GL(glDepthMask(true));
        RenderVisibleObjects(opaqueObjects);
        opaqueObjects.clear();
    }
    
    if (!transparentObjects.empty()) {
        
        std::sort(transparentObjects.begin(), transparentObjects.end(), SortTransparentObjects);
        
        ASSERT_GL(glEnable(GL_BLEND));
        ASSERT_GL(glDepthMask(false));
        //clipper.UseDepth = false;
        
        RenderTransparentVisibleObjects(transparentObjects);
        ASSERT_GL(glDepthMask(true));
        transparentObjects.clear();
    }
}

void RenderSystem::RenderVisibleObjects(const VisibleObjects& visibleObjects) {
    int currentVertexType = visibleObjects[0].vertexType;
    objectRenderers[currentVertexType]->Begin(false);
    objectRenderers[currentVertexType]->RenderObject(visibleObjects[0]);
    for (size_t i=1; i<visibleObjects.size(); ++i) {
        const VisibleObject& visibleObject = visibleObjects[i];
        if (currentVertexType!=visibleObject.vertexType) {
            objectRenderers[currentVertexType]->End(renderInfo);
            currentVertexType = visibleObject.vertexType;
            objectRenderers[currentVertexType]->Begin(false);
        }
        objectRenderers[currentVertexType]->RenderObject(visibleObject);
    }
    objectRenderers[currentVertexType]->End(renderInfo);
}

void RenderSystem::RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects) {
    int currentVertexType = visibleObjects[0].vertexType;
    objectRenderers[currentVertexType]->Begin(true);
    objectRenderers[currentVertexType]->RenderTransparentObject(visibleObjects[0]);
    for (size_t i=1; i<visibleObjects.size(); ++i) {
        const VisibleObject& visibleObject = visibleObjects[i];
        if (currentVertexType!=visibleObject.vertexType) {
            objectRenderers[currentVertexType]->End(renderInfo);
            currentVertexType = visibleObject.vertexType;
            objectRenderers[currentVertexType]->Begin(true);
        }
        objectRenderers[currentVertexType]->RenderTransparentObject(visibleObject);
    }
    objectRenderers[currentVertexType]->End(renderInfo);
}

void RenderSystem::Render() {
    if (!cameras) {
        return;
    }
    renderInfo.drawCalls = 0;
    renderInfo.verticesRendered = 0;
    renderInfo.objectsRendered = 0;
    for(auto camera : cameras->Objects()) {
        RenderCamera(camera);
    }
    
    /*static int counter = 0;
    if (counter++%300==0) {
        std::cout<<"Draw calls : " << renderInfo.drawCalls<< ", Vertices Rendered : " << renderInfo.verticesRendered<<", Objects rendered : "<< renderInfo.objectsRendered<<std::endl;
    }*/
}

bool RenderSystem::SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b) {

    if (a.orderable && b.orderable) {
        int orderA = a.orderable->Order();
        int orderB = b.orderable->Order();
        if (orderA!=orderB) {
            return orderA<orderB;
        }
    }

    if (a.vertexType != b.vertexType) {
        return a.vertexType<b.vertexType;
    }

    if (a.shader == b.shader) {
        if (a.texture==b.texture) {
            return a.distanceToCamera<b.distanceToCamera;
        } else {
            return a.texture<b.texture;
        }
    } else {
        return ((size_t)a.shader)<((size_t)b.shader);
    }
}

bool RenderSystem::SortTransparentObjects(const VisibleObject& a, const VisibleObject& b) {
    if (a.orderable && b.orderable) {
        int orderA = a.orderable->Order();
        int orderB = b.orderable->Order();
        if (orderA!=orderB) {
            return orderA<orderB;
        }
    }
    
    if (a.orderable && !b.orderable) {
        return false;
    } else if (!a.orderable && b.orderable) {
        return true;
    }

    return a.distanceToCamera>b.distanceToCamera;
}

void RenderSystem::SetCameras(Mini::RenderSystem::CameraSystem *cameraSystem) {
    cameras = cameraSystem;
}

Mini::RenderSystem::CameraSystem * RenderSystem::GetCameras() {
    return cameras;
}

Mini::RenderSystem::CameraSystem* RenderSystem::GetOriginalCameras() {
    return &scene->CreateSystem<CameraSystem>();
}
