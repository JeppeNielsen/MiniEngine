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

bool ObjectHasAncestor(GameObject go, GameObject ancestor) {
    while (true) {
        if (go == ancestor) return true;
        go = go.Hierarchy().Parent;
        if (!go) break;
    }
    return false;
}

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
    Parent.Changed.Bind([this] {
        GameObject current = Parent;
        assert(current!=owner);
        assert(!ObjectHasAncestor(current, owner));
        GameObject prev = Parent.PreviousValue();
        if (prev) {
            Hierarchy* h = prev.GetComponent<Hierarchy>();
            auto it = std::find(h->children.begin(), h->children.end(), owner);
            h->children.erase(it);
        }
        
        if (current) {
            current.GetComponent<Hierarchy>()->children.push_back(owner);
        }
    });
    Parent.Changed.MarkDefaults();
}

const ObjectCollection& Hierarchy::Children() {
    return children;
}
