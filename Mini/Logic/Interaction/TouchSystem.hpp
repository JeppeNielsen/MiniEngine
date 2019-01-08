//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "OctreeSystem.hpp"
#include "Scene.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Touchable.hpp"
#include "Orderable.hpp"
#include <set>
#include "Picker.hpp"
#include "InputManager.hpp"

namespace Mini {
    using namespace ECS;
    class TouchSystem : public System<Transform, Mesh, Touchable> {
    private:
        using OctreeSystem = OctreeSystem<Touchable>;
        struct CameraSystem : System<Transform, Camera> {};
        Picker picker;
    public:

        void Initialize() override;
        ~TouchSystem();
        OctreeSystem& Octree();
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
        void EnqueueDown(GameObject touchObject, TouchData touchData);
        
        int TouchDepth = 0;
        
        Property<InputManager*> Input;
    private:
                
        void TouchDown(TouchEvent e);
        void TouchUp(TouchEvent e);
        
        CameraSystem* cameras;
        OctreeSystem* octree;
        
        typedef std::vector<TouchData> Touched;
        
        Touched touches[12];
        
        Touched downs;
        Touched clicks;
        Touched ups;
        
        Touched equeuedDowns;
        
    public:
        void SetCameras(CameraSystem* cameraSystem);
        CameraSystem* GetCameras();
        CameraSystem* GetOriginalCameras();
        void FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough = false);
    private:
        
        using CancelledTouchables = std::set<Touchable*>;
        CancelledTouchables cancelledTouchables;
        
        void AddToTouchList(Touched &from, Touched &to);
        bool IsTouchInList(const TouchData &touchData, const Touched &list);
        void TouchableCancelled(Touchable *touchable);
        bool IsTouchValid(const TouchData& touchData);
    };
}
