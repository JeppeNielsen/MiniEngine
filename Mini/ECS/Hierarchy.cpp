//
//  Hierarchy.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 29/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "Hierarchy.hpp"
#include "Scene.hpp"

using namespace Mini::ECS;

Hierarchy::Hierarchy() {
    Initialize();
}

Hierarchy::Hierarchy(Hierarchy&& other) {
    owner = other.owner;
    children = other.children;
    other.children.clear();
    Initialize();
}

Hierarchy& Hierarchy::operator=(const Hierarchy & other) {
    owner = other.owner;
    children = other.children;
    Parent = other.Parent;
    Initialize();
    return *this;
}

void Hierarchy::Initialize() {
    static bool isFromHere = false;
    Parent.Changed.Bind([this] {
        if (isFromHere) {
            return;
        }
        GameObject current = Parent;
        if (ObjectHasAncestor(current, owner)) {
            GameObject prevValue = Parent.PreviousValue();
            isFromHere = true;
            Parent = prevValue;
            isFromHere = false;
            return;
        }
        GameObject prev = Parent.PreviousValue();
        if (prev) {
            Hierarchy* h = prev.GetComponent<Hierarchy>();
            auto it = std::find(h->children.begin(), h->children.end(), owner);
            h->children.erase(it);
            h->WorldEnabled.HasBecomeDirty.Unbind(this, &Hierarchy::MakeEnabledDirty);
        }
        
        if (current) {
            Hierarchy* h = current.GetComponent<Hierarchy>();
            h->children.push_back(owner);
            h->WorldEnabled.HasBecomeDirty.Bind(this, &Hierarchy::MakeEnabledDirty);
        }
        
        MakeEnabledDirty();
    });
    Parent.Changed.MarkDefaults();
    
    Enabled = true;
    Enabled.Changed.Bind(this, &Hierarchy::MakeEnabledDirty);
    Enabled.Changed.MarkDefaults();
    
    WorldEnabled.MakeDirty();
    WorldEnabled.Method = [this](bool& value) {
        const GameObject& parent = Parent();
        value = parent != nullptr ? parent.Hierarchy().WorldEnabled() : Enabled();
    };
    WorldEnabled.operator()();
}

const ObjectCollection& Hierarchy::Children() {
    return children;
}

void Hierarchy::MakeEnabledDirty() {
    bool wasWorldEnabled = WorldEnabled();
    WorldEnabled.MakeDirty();
    bool isWorldEnabled = WorldEnabled();
    if (wasWorldEnabled == isWorldEnabled) return;
    owner.Scene().SetEnable(owner.Id(), isWorldEnabled);
}

bool Hierarchy::ObjectHasAncestor(GameObject go, GameObject ancestor) {
    while (true) {
        if (!go) break;
        if (go == ancestor) return true;
        go = go.Hierarchy().Parent;
    }
    return false;
}
