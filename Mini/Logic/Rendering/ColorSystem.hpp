//
//  ColorSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Colorable.hpp"
#include <set>

namespace Mini {
  using namespace ECS;
  class ColorSystem : System<Mesh, Colorable> {
    public:
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void ColorChanged(GameObject object);
        void Update(float dt) override;
    private:
        std::set<GameObject> changedColorables;
    };
}
