//
//  VirtualTreeListSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "VirtualTreeList.hpp"
#include "Sizeable.hpp"
#include "Transform.hpp"

namespace Mini {
  class VirtualTreeListSystem : System<VirtualTreeList, Transform, Sizeable> {
    public:
        void Initialize();
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void Update(float dt);
    private:
        void UpdateVirtualList(GameObject object);
  };
}
