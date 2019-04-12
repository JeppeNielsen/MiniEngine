//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TouchSystem.hpp"
#include "Renderable.hpp"
#include "Orderable.hpp"

using namespace Mini;
using namespace ECS;

TouchSystem::TouchSystem(OctreeSystem& octree, CameraSystem& cameras, InputManager& input) :
    octree(octree), cameras(cameras), input(input) {
	input.TouchDown.Bind(this, &TouchSystem::TouchDown);
    input.TouchUp.Bind(this, &TouchSystem::TouchUp);
}

TouchSystem::~TouchSystem() {
    input.TouchDown.Unbind(this, &TouchSystem::TouchDown);
    input.TouchUp.Unbind(this, &TouchSystem::TouchUp);
}

void TouchSystem::ObjectAdded(GameObject object) {
    picker.TryAddClipper(object);
}

void TouchSystem::ObjectRemoved(GameObject object) {
    picker.TryRemoveClipper(object);
    
    Touchable* touchable = object.GetComponent<Touchable>();
    for(int i=0; i<12; ++i) {
        Touched& list = touches[i];
        for(int j=0; j<list.size(); ++j) {
            if (list[j].Touchable == touchable) {
                list.erase(list.begin() + j);
                --j;
                break;
            }
        }
    }
}

void TouchSystem::Update(float dt) {
    if (!cancelledTouchables.empty()) {
        for (CancelledTouchables::iterator it = cancelledTouchables.begin(); it!=cancelledTouchables.end(); ++it) {
            Touchable* cancelled = *it;
            for (int i=0; i<12; i++) {
                Touched& list = touches[i];
                for (size_t j=0; j<list.size(); j++) {
                    if (list[j].Touchable == cancelled) {
                        cancelled->Cancelled.Bind(this, &TouchSystem::TouchableCancelled);
                        ups.push_back(list[j]);
                        list.erase(list.begin()+j);
                        --j;
                    }
                }
            }
        }
        cancelledTouchables.clear();
    }
    
    for(auto& d : equeuedDowns) {
        downs.push_back(d);
    }
    equeuedDowns.clear();
    
    for (unsigned i=0; i<downs.size(); i++) {
        if (IsTouchValid(downs[i])) {
            downs[i].Touchable->Down(downs[i]);
        }
    }
    downs.clear();
    for (unsigned i=0; i<clicks.size(); i++) {
        if (IsTouchValid(clicks[i])) {
            clicks[i].Touchable->Click(clicks[i]);
        }
    }
    clicks.clear();
    for (unsigned i=0; i<ups.size(); i++) {
        if (IsTouchValid(ups[i])) {
            ups[i].Touchable->Up(ups[i]);
        }
    }
    ups.clear();
}

bool TouchSystem::IsTouchValid(const TouchData &touchData) {
    return !input.IsTouchSwallowed(touchData.Index, TouchDepth);
}

void TouchSystem::TouchDown(TouchEvent e) {
    Touched& list = touches[e.Index];
    
    FindTouchedObjects(list, e);
    AddToTouchList(list, downs);
    
    for (size_t i = 0; i<list.size(); i++) {
        list[i].Touchable->Cancelled.Bind(this, &TouchSystem::TouchableCancelled);
    }
    
    if (!list.empty() && !list[0].Touchable->ClickThrough) {
        input.SwallowTouch(e.Index, TouchDepth);
    }
}

void TouchSystem::TouchUp(TouchEvent e) {
    
    Touched& touchList = touches[e.Index];
    
    for (size_t i=0; i<touchList.size(); i++) {
        CancelledTouchables::iterator it = cancelledTouchables.find(touchList[i].Touchable);
        if (it!=cancelledTouchables.end()) {
            touchList.erase(touchList.begin() + i);
            i--;
        }
    }
    
    if (touchList.empty()) return;
    
    Touched list;
    FindTouchedObjects(list, e);
    
    for (unsigned i=0; i<list.size(); i++) {
        if (IsTouchInList(list[i], touchList)) {
            clicks.push_back(list[i]);
        }
    }
    
    AddToTouchList(touchList, ups);
    
    for (size_t i = 0; i<touchList.size(); i++) {
        touchList[i].Touchable->Cancelled.Unbind(this, &TouchSystem::TouchableCancelled);
    }
    
    touchList.clear();
}

void TouchSystem::FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough) {
    for(auto c : cameras.Objects()) {
        picker.Pick(c, list, e, forceClickThrough, [this] (const Ray& ray, ObjectCollection& list) {
            octree.GetObjectsAtRay(ray, list);
        }, &input);
    }
}

void TouchSystem::AddToTouchList(Touched &from, Touched &to) {
    for (unsigned i=0; i<from.size(); i++) {
        to.push_back(from[i]);
    }
}

bool TouchSystem::IsTouchInList(const TouchData &touchData, const Touched &list) {
    for (unsigned i=0; i<list.size(); i++) {
        if (list[i].Touchable == touchData.Touchable) return true;
    }
    return false;
}

void TouchSystem::TouchableCancelled(Touchable *touchable) {
    cancelledTouchables.insert(touchable);
}

void TouchSystem::EnqueueDown(GameObject touchObject, TouchData touchData) {
    touchData.object = touchObject;
    touchData.Touchable = touchObject.GetComponent<Touchable>();
    equeuedDowns.push_back(touchData);
    touches[touchData.Index].push_back(touchData);
}
