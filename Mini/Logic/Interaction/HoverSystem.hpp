//
//  HoverSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "OctreeSystem.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Hoverable.hpp"
#include "Picker.hpp"
#include "InputManager.hpp"

namespace Mini {
    using namespace ECS;
    struct HoverSystem : System<Transform, Mesh, Hoverable> {
    private:
        using OctreeSystem = OctreeSystem<Hoverable>;
        struct CameraSystem : System<Transform, Camera> {};
        Property<InputManager*> Input;
        Picker picker;
    public:
        void Initialize() override;
        OctreeSystem& Octree();
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
    private:
        CameraSystem* cameras;
        OctreeSystem* octree;
        using TouchList = std::vector<TouchData>;
        TouchList previousHovers;
    public:
        void SetCameras(CameraSystem* cameraSystem);
        CameraSystem* GetCameras();
        CameraSystem* GetOriginalCameras();
    };
}
