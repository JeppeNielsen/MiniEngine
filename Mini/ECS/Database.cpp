//
//  Database.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "Database.hpp"

using namespace Mini::ECS;

void Database::RemoveComponent(const GameObjectId objectId, const std::size_t componentId) {
    if (componentId<componentsIndexed.size() && componentsIndexed[componentId]) {
        componentsIndexed[componentId]->Destroy(objectId);
    }
}

void Database::RemoveAllComponents(const GameObjectId objectId) {
    for(auto& component : components) {
        if (component->Contains(objectId)) {
            component->Destroy(objectId);
        }
    }
}

bool Database::GameObjectIdValid(const GameObjectId object) const {
    const auto pos = GameObjectId(object & GameObjectIdIndexMask);
    return (pos < objects.size() && objects[pos] == object);
}

GameObjectId Database::CreateObject() {
    GameObjectId object;

    if(available) {
        const auto objectIndex = next;
        const auto version = objects[objectIndex] & (~GameObjectIdIndexMask);

        object = objectIndex | version;
        next = objects[objectIndex] & GameObjectIdIndexMask;
        objects[objectIndex] = object;
        --available;
    } else {
        object = GameObjectId(objects.size());
        objects.push_back(object);
    }
    return object;
}

void Database::Remove(const GameObjectId objectId) {
    const auto object = objectId & GameObjectIdIndexMask;
    const auto version = (((objectId >> 20) + 1) & GameObjectIdIndexMask) << 20;
    const auto node = (available ? next : ((object + 1) & GameObjectIdIndexMask)) | version;

    objects[object] = node;
    next = object;
    ++available;
}

bool Database::TryFindComponentContainer(const std::string& componentName, IContainer** container, int& componentId) {
    for (auto i = 0; i<componentNames.size(); ++i) {
        if (componentNames[i] == componentName) {
            for(auto o = 0; o<componentsIndexed.size(); ++o) {
                if (componentsIndexed[o].get() == components[i]) {
                    *container = components[i];
                    componentId = o;
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<Database::NameIndex> Database::GetComponentNameIndices() {
    std::vector<Database::NameIndex> indicies;
    for (int i = 0; i<components.size(); ++i) {
        int foundIndex = -1;
        for(int o = 0; o<componentsIndexed.size(); ++o) {
            if (componentsIndexed[o].get() == components[i]) {
                foundIndex = o;
                break;
            }
        }
        indicies.push_back(std::make_pair(componentNames[i], foundIndex));
    }
    return indicies;
}

void Database::AddCustomComponent(int id, IContainer* container, const std::string& name) {
    if (id>=componentsIndexed.size()) {
        componentsIndexed.resize(id + 1);
    }
    componentsIndexed[id] = std::unique_ptr<IContainer>(container);
    components.push_back(componentsIndexed[id].get());
    componentNames.push_back(name);
}

void Database::RemoveCustomComponent(int id) {
    for (size_t i = 0; i<components.size(); i++) {
        if (components[i] == componentsIndexed[id].get()) {
            components.erase(components.begin() + i);
            componentNames.erase(componentNames.begin() + i);
            break;
        }
    }
    componentsIndexed[id] = nullptr;
}
