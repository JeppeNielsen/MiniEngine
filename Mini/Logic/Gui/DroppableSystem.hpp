//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "Droppable.hpp"
#include "DroppableSystem.hpp"
#include "TouchSystem.hpp"

namespace Mini {
    using namespace ECS;
    class DroppableSystem : System<Droppable, Touchable> {
    public:
        void Initialize();
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void Update(float dt);
    private:
    
        void TouchDown(TouchData d, GameObject object);
        void DropStarted(TouchData d, GameObject object);
        void TouchUp(TouchData d, GameObject object);
        TouchSystem* touchSystem;
        
        struct DownObject {
            float maxMovement;
            TouchData touchData;
            Vector2 prevPosition;
            GameObject createdObject;
        };
        
        std::map<GameObject, GameObject> objectToClone;
        std::map<GameObject, DownObject> downObjects;
    };
}
