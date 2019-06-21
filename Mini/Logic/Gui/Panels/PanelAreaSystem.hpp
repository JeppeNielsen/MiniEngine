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
#include "PanelSplitterSystem.hpp"

namespace Mini {
    class Gui;
    class PanelSystem;
    using namespace ECS;
    struct PanelAreaSystem : System<PanelArea> {
    public:
        PanelAreaSystem(PanelSystem& panels, PanelSplitterSystem& panelSplitter, Gui& gui);
        void Update(float dt) override;
        void CreateSplitters(GameObject object, PanelArea* area);
    private:
        PanelSystem& panels;
        PanelSplitterSystem& panelSplitter;
        Gui& gui;
    };
}
