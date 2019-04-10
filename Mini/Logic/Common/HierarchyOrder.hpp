//
//  HierarchyOrder.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Scene.hpp"
#include "Orderable.hpp"
#include <set>

namespace Mini {
    using namespace ECS;
    struct HierarchyOrder : System<Orderable> {
    public:
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void SetDirty();
        void Update(float dt);
        void CalculateOrder(int& orderOffset, GameObject object);
    private:
        bool orderIsDirty;
    };
}
