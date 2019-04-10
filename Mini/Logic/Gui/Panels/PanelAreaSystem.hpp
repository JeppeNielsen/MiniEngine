//
//  PanelAreaSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "PanelArea.hpp"
#include <set>
#include "Draggable.hpp"
#include "Transform.hpp"
#include "PanelLocation.hpp"

namespace Mini {
    class Gui;
    class PanelSystem;
    using namespace ECS;
    
    struct PanelSplitter {
        GameObject area;
        PanelLocation location;
        bool isHorizontal;
    };

    struct PanelAreaSystem : System<PanelArea> {
        PanelSystem* panels;
        Gui* gui;
        void Initialize() override;
        void Update(float dt) override;
        void CreateSplitters(GameObject object, PanelArea* area);
        
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
    };
}
