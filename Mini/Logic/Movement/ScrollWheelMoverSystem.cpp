//
//  ScrollWheelMoverSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ScrollWheelMoverSystem.hpp"
#include "InputManager.hpp"

using namespace Mini;

void ScrollWheelMoverSystem::Initialize() {
    Input.Changed.Bind([this]() {
        if (Input.PreviousValue()) {
            Input.PreviousValue()->ScrollChanged.Unbind(this, &ScrollWheelMoverSystem::ScrollChanged);
        }
        if (Input()) {
            Input()->ScrollChanged.Bind(this, &ScrollWheelMoverSystem::ScrollChanged);
        }
    });
    
    currentScrollValue = 0;
}

ScrollWheelMoverSystem::~ScrollWheelMoverSystem() {
    Input = nullptr;
}

void ScrollWheelMoverSystem::ObjectAdded(GameObject object) {
    object.GetComponent<Hoverable>()->Enter.Bind(this, &ScrollWheelMoverSystem::Enter, object);
    object.GetComponent<Hoverable>()->Exit.Bind(this, &ScrollWheelMoverSystem::Exit, object);
}

void ScrollWheelMoverSystem::ObjectRemoved(GameObject object) {
    object.GetComponent<Hoverable>()->Enter.Unbind(this, &ScrollWheelMoverSystem::Enter, object);
    object.GetComponent<Hoverable>()->Exit.Unbind(this, &ScrollWheelMoverSystem::Exit, object);
}

void ScrollWheelMoverSystem::Enter(TouchData d, GameObject object) {
    activeObjects.push_back(object);
}

void ScrollWheelMoverSystem::Exit(TouchData d, GameObject object) {
    auto it = std::find(activeObjects.begin(), activeObjects.end(), object);
    if (it!=activeObjects.end()) {
        activeObjects.erase(it);
    }
}

void ScrollWheelMoverSystem::Update(float dt) {
    if (currentScrollValue!=0) {
    
        for(auto o : activeObjects) {
            ScrollWheelMover* mover = o.GetComponent<ScrollWheelMover>();
            o.GetComponent<Transform>()->Position += mover->Movement() * mover->Speed() * currentScrollValue * dt;
        }
    }
    
    currentScrollValue *= 0.8f;
}

void ScrollWheelMoverSystem::ScrollChanged(float delta) {
    currentScrollValue = delta;
}
