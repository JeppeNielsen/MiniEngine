//
//  ScrollWheelMoverSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "Property.hpp"
#include "ScrollWheelMover.hpp"
#include "Hoverable.hpp"

namespace Mini {
    using namespace ECS;
    class ScrollWheelMoverSystem : System<Transform, ScrollWheelMover, Hoverable> {
        public:
            ~ScrollWheelMoverSystem();
            void Initialize() override;
            void ObjectAdded(GameObject object) override;
            void ObjectRemoved(GameObject object) override;
            void Update(float dt) override;
        
            Property<InputManager*> Input;
        
        private:
            void Enter(TouchData d, GameObject object);
            void Exit(TouchData d, GameObject object);
        
            void ScrollChanged(float delta);
        
            float currentScrollValue;
        
            ObjectCollection activeObjects;
        
    };
}


