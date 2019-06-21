//
//  PanelSplitterSystem.hpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 14/04/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"
#include "PanelSplitter.hpp"
#include "Draggable.hpp"
#include "Transform.hpp"

namespace Mini {
    using namespace ECS;
    struct PanelSplitterSystem : System<PanelSplitter, Draggable, Transform> {
        std::set<GameObject> splittersNeedingAlignment;
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void AreaSizeChanged(GameObject object);
        void SplitValueChanged(std::string id, GameObject object);
        void Update(float dt) override;
        void AlignSplitter(GameObject object);
        void SetSplitValueFromTransform(GameObject object);
    };
}
