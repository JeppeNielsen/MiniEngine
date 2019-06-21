//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"
#include "Property.hpp"
#include "OctreeSystem.hpp"
#include "CameraSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Touchable.hpp"
#include "Picker.hpp"
#include "InputManager.hpp"
#include <vector>

namespace Mini {
    using namespace ECS;
    
    struct TouchSystem : System<Transform, Mesh, Touchable> {
    public:
        using OctreeSystem = OctreeSystem<Touchable>;
    
        TouchSystem(OctreeSystem& octree, CameraSystem& cameras, InputManager& input);
        ~TouchSystem();
        
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
        void EnqueueDown(GameObject touchObject, TouchData touchData);
        OctreeSystem& Octree();
        
        int TouchDepth = 0;
    private:
                
        void TouchDown(TouchEvent e);
        void TouchUp(TouchEvent e);
        
        OctreeSystem& octree;
        CameraSystem& cameras;
        InputManager& input;
        
        typedef std::vector<TouchData> Touched;
        
        Touched touches[12];
        
        Touched downs;
        Touched clicks;
        Touched ups;
        
        Touched equeuedDowns;
        
        Picker picker;
    public:
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
