//
//  Scene.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "Scene.hpp"

using namespace Mini::ECS;

Scene::Scene(Database& database)
: database{ database } { }

Scene::~Scene() {
    for(const auto object : objects.objects) {
        RemoveObjectFromDatabase(object);
    }
}

GameObject Scene::CreateObject() {
    const auto object = database.CreateObject();
    objects.Add(object);
    GameObject go = GameObject(*this, object);
    go.AddComponent<Hierarchy>()->owner = go;
    return go;
}

void Scene::Update(float dt) {
    DoActions(addComponentActions, [this] (const auto& pair) {
        if (pair.second<componentSystemLists.size()) {
            auto& systemsUsingComponent = componentSystemLists[pair.second];
            for(const auto& system : systemsUsingComponent) {
                if (!system->objects.Contains(pair.first)) {
                    system->TryAddObject(pair.first);
                }
            }
        }
    });
    
    DoActions(enableObjectActions, [this](const auto object) {
        for(const auto& system : systems) {
            if (!system->objects.Contains(object)) {
                system->TryAddObject(object);
            }
        }
    });
    
    DoActions(disableObjectActions, [this](const auto object) {
        for(const auto& system : systems) {
            if (system->objects.Contains(object)) {
                system->RemoveObject(object);
            }
        }
    });
    
    DoActions(removeComponentActions, [this] (const auto& pair) {
        if (pair.second<componentSystemLists.size()) {
            auto& systemsUsingComponent = componentSystemLists[pair.second];
            for(const auto& system : systemsUsingComponent) {
                if (system->objects.Contains(pair.first)) {
                    system->RemoveObject(pair.first);
                }
            }
        }
        database.RemoveComponent(pair.first, pair.second);
    });
    
    DoActions(removeActions, [this] (const auto object) {
        database.GetComponent<Hierarchy>(object)->Parent = GameObject();
        RemoveObjectFromDatabase(object);
        objects.Remove(object);
    });
    
    UpdateSystems(dt);
}

Database& Scene::GetDatabase() const {
    return database;
}

bool Scene::IsEmpty() const {
    return objects.objects.empty();
}

int Scene::GetMaxSystemIndex() const {
    return (int)systemsIndexed.size();
}

void Scene::RemoveObject(const GameObjectId object) {
    removeActions.insert(object);
}

void Scene::UpdateSystems(float dt) {
    for(const auto& system : systems) {
        system->Update(dt);
    }
}

void Scene::RemoveObjectFromDatabase(const GameObjectId object) {
    for(const auto& system : systems) {
        if (system->objects.Contains(object)) {
            system->RemoveObject(object);
        }
    }
    database.RemoveAllComponents(object);
    database.Remove(object);
}

void Scene::AddComponent(GameObjectId objectId, const std::size_t componentId) {
    if (disableObjectActions.find(objectId) == disableObjectActions.end()) {
        addComponentActions.insert(std::make_pair(objectId, componentId));
    }
    database.componentsIndexed[componentId]->CreateDefault(objectId);
}

void* Scene::GetComponent(GameObjectId objectId, const std::size_t componentId) {
    return database.componentsIndexed[componentId]->GetInstance(objectId);
}

void Scene::RemoveComponent(GameObjectId objectId, const std::size_t componentId) {
    removeComponentActions.insert(std::make_pair(objectId, componentId));
}

void Scene::AddCustomSystem(int systemId, ISystem* system) {
    if (systemId>=systemsIndexed.size()) {
        systemsIndexed.resize(systemId + 1);
    }
    if (!systemsIndexed[systemId]) {
        systemsIndexed[systemId] = std::unique_ptr<ISystem>(system);
        systemsIndexed[systemId]->InitializeComponents(this, componentSystemLists);
        systems.push_back(systemsIndexed[systemId].get());
    }
}

void Scene::RemoveCustomSystem(int systemId) {
    for(size_t i=0; i<systems.size(); ++i ) {
        if (systems[i] == systemsIndexed[systemId].get()) {
            systems.erase(systems.begin() + i);
            break;
        }
    }
    systemsIndexed[systemId] = nullptr;
}

const GameObjectIterator Scene::Objects() {
    return GameObjectIterator(*this, objects.objects, 0);
}

void Scene::SetEnable(const GameObjectId object, bool enable) {
    if (enable) {
        EnableAction(enableObjectActions, disableObjectActions, object);
    } else {
        auto it = addComponentActions.begin();
        while (it != addComponentActions.end()) {
            if (it->first == object) {
                it = addComponentActions.erase(it);
            } else {
                ++it;
            }
        }
        EnableAction(disableObjectActions, enableObjectActions, object);
    }
}

void Scene::EnableAction(Actions &on, Actions &off, const GameObjectId object) {
    on.insert(object);
    const auto it = std::find(off.begin(), off.end(), object);
    if (it != off.end()) {
        off.erase(it);
    }
}
