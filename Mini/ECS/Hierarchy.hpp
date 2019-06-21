//
//  Hierarchy.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 29/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include "Property.hpp"
#include "DirtyProperty.hpp"

namespace Mini {
namespace ECS {
    class Scene;
    class Hierarchy {
    public:
        Hierarchy();
        Hierarchy(Hierarchy&&);
        Hierarchy& operator=(const Hierarchy&);
        
        Property<GameObject> Parent;
        
        const ObjectCollection& Children();
        
        Property<bool> Enabled;
        
        template<typename Func>
        void IterateChildren(Func&& func) {
            IterateChildrenInternal(owner, func);
        }
        
        DirtyProperty<bool> WorldEnabled;
    
    private:
        template<typename Func>
        void IterateChildrenInternal(GameObject go, Func&& func) {
            func(go);
            auto& hierarchy = go.Hierarchy();
            for(auto child : hierarchy.Children()) {
                IterateChildrenInternal(child, func);
            }
        }
        
        void Initialize();
        void MakeEnabledDirty();
        
        GameObject owner;
        ObjectCollection children;
        
        friend class Scene;
    };
}
}
