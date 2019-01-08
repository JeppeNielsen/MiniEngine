//
//  TouchCancelSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Scene.hpp"
#include <map>
#include "Touchable.hpp"
#include "TouchableCanceller.hpp"
#include "Transform.hpp"

namespace Mini {
    using namespace ECS;
    class TouchCancelSystem : public System<Touchable, TouchableCanceller, Transform> {
    public:
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
    private:
        struct ActiveTouchable {
            Touchable* touchable;
            Transform* transform;
            TouchableCanceller* canceller;
            float distanceTravelled;
            Vector3 prevPosition;
            TouchData touch;
        };
        
        typedef std::map<Touchable*, ActiveTouchable> ActiveTouchables;
        ActiveTouchables activeTouchables;
        
        void TouchableDown(TouchData touch, GameObject object);
        void TouchableUp(TouchData touch, GameObject object);
    };
}
