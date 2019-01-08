//
//  HoverSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "HoverSystem.hpp"

using namespace Mini;

void HoverSystem::Initialize() {
    octree = &scene->CreateSystem<OctreeSystem>();
    cameras = &scene->CreateSystem<CameraSystem>();
}

HoverSystem::OctreeSystem& HoverSystem::Octree() { return *octree; }

void HoverSystem::ObjectAdded(GameObject object) {
    picker.TryAddClipper(object);
}

void HoverSystem::ObjectRemoved(GameObject object) {
    picker.TryRemoveClipper(object);
}

void HoverSystem::Update(float dt) {
    TouchList current;
    
    Vector2 pos = Input()->GetTouchPosition(0);
    
    TouchEvent e(0, pos);
    
    for(auto c : cameras->Objects()) {
        picker.Pick(c, current, e, false, [this] (const Ray& ray, ObjectCollection& list) {
            octree->GetObjectsAtRay(ray, list);
        }, Input());
    }
    
    
    for (auto& c : current) {
        bool isInPrev = false;
        for(auto &p : previousHovers) {
            if (c.object == p.object) {
                isInPrev = true;
                break;
            }
        }
        
        if (!isInPrev) {
            c.object.GetComponent<Hoverable>()->Enter(c);
        }
    }
    
    for (auto& p : previousHovers) {
        bool isInCurrent = false;
        for(auto &c : current) {
            if (p.object == c.object) {
                isInCurrent = true;
                break;
            }
        }
        
        if (!isInCurrent) {
            p.object.GetComponent<Hoverable>()->Exit(p);
        }
    }
    
    previousHovers = current;
}

void HoverSystem::SetCameras(HoverSystem::CameraSystem *cameraSystem) {
    cameras = cameraSystem;
}

HoverSystem::CameraSystem * HoverSystem::GetCameras() {
    return cameras;
}

HoverSystem::CameraSystem* HoverSystem::GetOriginalCameras() {
    return &scene->CreateSystem<CameraSystem>();
}
