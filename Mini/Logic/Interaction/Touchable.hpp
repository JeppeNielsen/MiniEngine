//
//  Touchable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/16/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Mesh.hpp"
#include "GameObject.hpp"

namespace Mini {
    using namespace ECS;
    
    class Camera;
    class Touchable;
    class InputManager;
    
    struct TouchData {
        GameObject object;
        Touchable* Touchable;
        InputManager* Input;
        int Index;
        Vector2 Position;
        Vector3 WorldPosition;
        size_t TriangleIndex;
        Transform* CameraTransform;
        Camera* Camera;
        Vector3 WorldNormal;
        Ray Ray;
    };
    
	class Touchable {
	public:
        void Cancel();
		
        Event<TouchData> Down;
        Event<TouchData> Up;
        Event<TouchData> Click;
    
        bool ClickThrough = false;
    
        Event<Touchable*> Cancelled;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(ClickThrough);
        TYPE_FIELDS_END
	};
}
