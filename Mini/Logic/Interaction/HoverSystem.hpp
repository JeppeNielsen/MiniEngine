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
#include "CameraSystem.hpp"
#include "InputManager.hpp"

namespace Mini {
    using namespace ECS;
    struct HoverSystem : System<Transform, Mesh, Hoverable> {
    public:
        using OctreeSystem = OctreeSystem<Hoverable>;
        
        HoverSystem(OctreeSystem& octree, CameraSystem& cameras, InputManager& input);
        
        Picker picker;
    public:
        OctreeSystem& Octree();
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
    private:
        
        OctreeSystem& octree;
        CameraSystem& cameras;
        InputManager& input;
        using TouchList = std::vector<TouchData>;
        TouchList previousHovers;
    };
}
