//
//  FirstPersonMoverSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "ECS.hpp"
#include "Property.hpp"
#include "InputManager.hpp"
#include "Transform.hpp"
#include "FirstPersonMover.hpp"
#include "DraggableSystem.hpp"
#include <map>

namespace Mini {
    using namespace ECS;
    class DraggableSystem;
    struct FirstPersonMoverSystem : System<Transform, FirstPersonMover>  {
    public:
        FirstPersonMoverSystem(InputManager& input, DraggableSystem* draggableSystem = nullptr);
        ~FirstPersonMoverSystem();
        
        void Update(float dt);
private:
        void TouchDown(TouchEvent e);
        void TouchUp(TouchEvent e);
        
        void UpdateMovement(float dt, GameObject object);
        bool UpdateRotation(float dt, GameObject object);
        
        typedef std::map<int, Vector2> Touches;
        Touches touches;

        InputManager& input;
        DraggableSystem* draggableSystem;
    };
}
