#pragma once
#include "System.hpp"
#include "Transform.hpp"

namespace Mini {
    using namespace ECS;
	class TransformHierarchy : public System<Transform> {
    public:
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void ParentChanged(GameObject object);
        void HookParent(Transform* transform, GameObject parent);
    };
}
