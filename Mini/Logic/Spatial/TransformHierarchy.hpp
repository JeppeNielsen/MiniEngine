#pragma once
#include "System.hpp"
#include "Transform.hpp"

namespace Mini {
    using namespace ECS;
	struct TransformHierarchy : System<Transform> {
    protected:
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void ParentChanged(GameObject object);
        void HookParent(Transform* transform, GameObject parent);
    };
}
