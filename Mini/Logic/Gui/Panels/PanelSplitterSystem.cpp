//
//  PanelSplitterSystem.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 14/04/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "PanelSplitterSystem.hpp"
#include "Sizeable.hpp"
#include "PanelArea.hpp"

using namespace Mini;
using namespace Mini::ECS;

void PanelSplitterSystem::ObjectAdded(GameObject object) {
    object.GetComponent<PanelSplitter>()->area.GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelSplitterSystem::AreaSizeChanged, object);
    object.GetComponent<PanelSplitter>()->area.GetComponent<PanelArea>()->SplitValueChanged.Bind(this, &PanelSplitterSystem::SplitValueChanged, object);
    splittersNeedingAlignment.insert(object);
}

void PanelSplitterSystem::ObjectRemoved(GameObject object) {
    object.GetComponent<PanelSplitter>()->area.GetComponent<Sizeable>()->Size.Changed.Unbind(this, &PanelSplitterSystem::AreaSizeChanged, object);
    object.GetComponent<PanelSplitter>()->area.GetComponent<PanelArea>()->SplitValueChanged.Unbind(this, &PanelSplitterSystem::SplitValueChanged, object);
    auto it = splittersNeedingAlignment.find(object);
    if (it!=splittersNeedingAlignment.end()) {
        splittersNeedingAlignment.erase(it);
    }
}

void PanelSplitterSystem::AreaSizeChanged(GameObject object) {
    splittersNeedingAlignment.insert(object);
}

void PanelSplitterSystem::SplitValueChanged(std::string id, GameObject object) {
    splittersNeedingAlignment.insert(object);
}

void PanelSplitterSystem::Update(float dt) {
    for(auto o : Objects()) {
        if (o.GetComponent<Draggable>()->IsDragging) {
            SetSplitValueFromTransform(o);
        }
    }

    for(auto o : splittersNeedingAlignment) {
        AlignSplitter(o);
    }
    splittersNeedingAlignment.clear();
}

void PanelSplitterSystem::AlignSplitter(GameObject object) {
    PanelSplitter* splitter = object.GetComponent<PanelSplitter>();
    Sizeable* areaSize = splitter->area.GetComponent<Sizeable>();
    PanelArea* area = splitter->area.GetComponent<PanelArea>();
    Transform* transform = object.GetComponent<Transform>();
    Sizeable* sizeable = object.GetComponent<Sizeable>();
    
    float splitValue = area->GetSplitValue(splitter->location.Id());
    
    Rect rect = splitter->location.GetRect(areaSize->Size, [area] (const std::string& id) {
       return area->GetSplitValue(id);
    });
    
    if (splitter->isHorizontal) {
        transform->Position = { rect.x + rect.width * splitValue-5, rect.y };
        sizeable->Size = { 10, rect.height };
    } else {
        transform->Position = { rect.x,  rect.y + rect.height * splitValue-5 };
        sizeable->Size = { rect.width, 10 };
    }
}

void PanelSplitterSystem::SetSplitValueFromTransform(GameObject object) {
    PanelSplitter* splitter = object.GetComponent<PanelSplitter>();
    Sizeable* areaSize = splitter->area.GetComponent<Sizeable>();
    PanelArea* area = splitter->area.GetComponent<PanelArea>();
    Transform* transform = object.GetComponent<Transform>();
    
    Rect rect = splitter->location.GetRect(areaSize->Size, [area] (const std::string& id) {
       return area->GetSplitValue(id);
    });
    
    float splitValue;
    
    if (splitter->isHorizontal) {
        splitValue = (transform->Position().x - rect.x + 5) / rect.width;
    } else {
        splitValue = (transform->Position().y - rect.y + 5) / rect.height;
    }
    
    area->SetSplitValue(splitter->location.Id(), splitValue);
}
