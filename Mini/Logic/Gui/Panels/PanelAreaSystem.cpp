//
//  PanelAreaSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelAreaSystem.hpp"
#include "PanelSystem.hpp"
#include "Gui.hpp"

using namespace Mini;

PanelAreaSystem::PanelAreaSystem(PanelSystem& panels, PanelSplitterSystem& panelSplitter, Gui& gui) :
panels(panels), panelSplitter(panelSplitter), gui(gui) { }

void PanelAreaSystem::Update(float dt) {
    for(auto o : Objects()) {
        PanelArea* area = o.GetComponent<PanelArea>();
        if (area->IsDirty) {
            area->IsDirty = false;
            CreateSplitters(o, area);
        }
    }
}

void PanelAreaSystem::CreateSplitters(GameObject object, PanelArea* area) {
    for(auto o : object.Hierarchy().Children()) {
        if (o.GetComponent<PanelSplitter>()) {
            o.Remove();
        }
    }
    
    std::map<std::string, float> newSplits;
    
    for(auto o : panels.Objects()) {
        Panel* panel = o.GetComponent<Panel>();
        GameObject panelArea = panel->Area;
        if (panelArea!=object) continue;
        
        panel->location.RecurseLocations([this, &newSplits, area, object, panelArea, panel] (const PanelLocation& location) {
            PanelLocation splitLocation = location.Split();
            std::string id = splitLocation.Id();
            
            if (newSplits.find(id)!=newSplits.end()) return;
            
            auto it = area->Splits.find(id);
            if (it != area->Splits.end()) {
                newSplits[id] = it->second;
            } else {
                newSplits[id] = 0.5f;
            }
            
            GameObject splitter = gui.CreateControl(object, "Box");
            splitter.AddComponent<PanelSplitter>()->location = splitLocation;
            splitter.GetComponent<PanelSplitter>()->isHorizontal = location.IsHorizontal();
            splitter.GetComponent<PanelSplitter>()->area = panelArea;
            splitter.AddComponent<Draggable>()->Movement = location.IsHorizontal() ? Draggable::MovementMode::XAxis : Draggable::MovementMode::YAxis;
            splitter.RemoveComponent<Renderable>();
        });
        
        
    }
    
    area->Splits = newSplits;
}


