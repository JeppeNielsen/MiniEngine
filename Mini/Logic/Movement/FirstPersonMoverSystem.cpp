//
//  FirstPersonMoverSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "FirstPersonMoverSystem.hpp"
#include "MathHelper.hpp"

using namespace Mini;

FirstPersonMoverSystem::FirstPersonMoverSystem() : draggableSystem(0) {
    isDraggableSystemChecked = false;
}

void FirstPersonMoverSystem::Initialize() {
    Input.Changed.Bind([this]() {
        if (!Input()) return;
        Input()->TouchDown.Bind(this, &FirstPersonMoverSystem::TouchDown);
        Input()->TouchUp.Bind(this, &FirstPersonMoverSystem::TouchUp);
    });
}

void FirstPersonMoverSystem::Destroy() {
    Input()->TouchDown.Unbind(this, &FirstPersonMoverSystem::TouchDown);
    Input()->TouchUp.Unbind(this, &FirstPersonMoverSystem::TouchUp);
}

void FirstPersonMoverSystem::TouchDown(TouchEvent e) {
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(e.Index)) return;
    touches[e.Index] = e.Position;
    
    if (e.Index==1) {
        for (auto object : Objects()) {
            Transform* transform = object.GetComponent<Transform>();
            FirstPersonMover* mover = object.GetComponent<FirstPersonMover>();
            mover->initialRotation = transform->Rotation();
        }
    }
}

void FirstPersonMoverSystem::TouchUp(TouchEvent e) {
    Touches::iterator it = touches.find(e.Index);
    if (it!=touches.end()) touches.erase(it);
}

void FirstPersonMoverSystem::Update(float dt) {
    if (!isDraggableSystemChecked) {
        isDraggableSystemChecked = true;
        draggableSystem = &scene->CreateSystem<DraggableSystem>();
    }
    
    for(auto o : Objects()) {
        if (!UpdateRotation(dt, o)) {
            UpdateMovement(dt, o);
        }
    }
}

void FirstPersonMoverSystem::UpdateMovement(float dt, GameObject object) {
    int touchIndex = object.GetComponent<FirstPersonMover>()->MovementTouchIndex;
    if (draggableSystem && draggableSystem->IsTouchIndexUsed(touchIndex)) return;
    if (Input()->IsTouchSwallowed(touchIndex, 0)) return;
    
    Touches::iterator it = touches.find(touchIndex);
    if (it==touches.end()) return;
    
    Vector2 delta = Input()->GetTouchPosition(touchIndex) - it->second;
    
    Transform* transform = object.GetComponent<Transform>();
    FirstPersonMover* mover = object.GetComponent<FirstPersonMover>();
    Vector3 vector = transform->World().TransformVector(Vector3(delta.x * dt * mover->MovementSpeed,0,-delta.y * dt * mover->MovementSpeed));
    transform->Position += vector;
}

bool FirstPersonMoverSystem::UpdateRotation(float dt, GameObject object) {
    int touchIndex = object.GetComponent<FirstPersonMover>()->RotationTouchIndex;
    if (Input()->IsTouchSwallowed(touchIndex, 0)) return false;
    
    Touches::iterator it = touches.find(touchIndex);
    if (it==touches.end()) return false;
    
    Vector2 delta = Input()->GetTouchPosition(touchIndex) - it->second;
    
    Transform* transform = object.GetComponent<Transform>();
    FirstPersonMover* mover = object.GetComponent<FirstPersonMover>();
    float maxPitch = ToRadians(mover->MaxPitch);
    float rotationSpeed = ToRadians(mover->RotationSpeed);
            
    float xRot = delta.y * rotationSpeed;
    float yRot = -delta.x * rotationSpeed;
    
    if (xRot<-maxPitch) {
        xRot = -maxPitch;
    } else if (xRot>maxPitch) {
        xRot = maxPitch;
    }
    
    Quaternion xAxis = Quaternion::Create(xRot, Vector3(1,0,0));
    Quaternion yAxis = Quaternion::Create(yRot, Vector3(0,1,0));
    
    Quaternion rot = yAxis * mover->initialRotation * xAxis;
    
    rot.Normalize();
    
    Vector3 euler = rot.ToEuler();
    float pitch = euler.x;
    if (pitch<-maxPitch) {
        rot = yAxis * mover->initialRotation * Quaternion::Create(-maxPitch-mover->initialRotation.ToEuler().x, Vector3(1,0,0));
    } else if (pitch>maxPitch) {
        Quaternion min;
        rot = yAxis * mover->initialRotation * Quaternion::Create(maxPitch-mover->initialRotation.ToEuler().x, Vector3(1,0,0));
    }
    
    transform->Rotation = rot;
	return true;
}






