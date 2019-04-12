//
//  Scene.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Database.hpp"
#include "System.hpp"
#include <set>
#include <assert.h>

namespace Mini {
namespace ECS {

class Scene {
    using SystemIdHelper = IdHelper<struct SystemIdHelper>;
    using SystemsIndexed = std::vector<std::unique_ptr<ISystem>>;
    using Systems = std::vector<ISystem*>;
    using Actions = std::set<GameObjectId>;
    using RemoveComponentActions = std::set<std::pair<GameObjectId, size_t>>;
    
    friend class GameObject;
    
public:
    Scene(Database& database);
    ~Scene();

    template<typename T, typename...Args>
    T& CreateSystem(Args&&... args) {
        const auto systemId = SystemIdHelper::GetId<T>();
        if (systemId>=systemsIndexed.size()) {
            systemsIndexed.resize(systemId + 1);
        }
        if (!systemsIndexed[systemId]) {
            systemsIndexed[systemId] = std::make_unique<T>(args...);
            T& system = static_cast<T&>(*systemsIndexed[systemId]);
            system.InitializeComponents(*this, componentSystemLists);
            systems.push_back(&system);
            return system;
        } else {
            return static_cast<T&>(*systemsIndexed[systemId]);
        }
    }
    
    void AddCustomSystem(int systemId, ISystem* system);
    void RemoveCustomSystem(int systemId);
    
    GameObject CreateObject();
    
    void Update(float dt);
    
    Database& GetDatabase() const;
    
    bool IsEmpty() const;
    
    int GetMaxSystemIndex() const;
    
    const GameObjectIterator Objects();
    
private:
    
    void RemoveObject(const GameObjectId object);
    
    template<typename T, typename...Args>
    T* AddComponent(const GameObjectId object, Args&& ... args) {
        addComponentActions.insert(object);
        return database.AddComponent<T>(object, args...);
    }
    
    template<typename T>
    T* AddReferenceComponent(const GameObjectId object, const GameObjectId referenceId) {
        addComponentActions.insert(object);
        return database.AddReferenceComponent<T>(object, referenceId);
    }
    
    template<typename T>
    void RemoveComponent(const GameObjectId objectId) {
        const auto componentId = Database::IdHelper::GetId<T>();
        RemoveComponent(objectId, componentId);
    }
    
    template<typename Actions, typename Func>
    void DoActions(Actions& actions, Func&& func) {
        if (actions.empty()) return;
        for(const auto& object : actions)  {
            func(object);
        }
        actions.clear();
    }
    
    void AddComponent(GameObjectId objectId, const std::size_t componentId);
    void* GetComponent(GameObjectId objectId, const std::size_t componentId);
    void RemoveComponent(GameObjectId objectId, const std::size_t componentId);
    
    void UpdateSystems(float dt);
    void RemoveObjectFromDatabase(const GameObjectId object);

    Database& database;
    SystemsIndexed systemsIndexed;
    Systems systems;
    ISystem::ComponentSystemLists componentSystemLists;
    
    ObjectList objects;
    Actions addComponentActions;
    RemoveComponentActions removeComponentActions;
    Actions removeActions;
};
  
template<typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args) const {
    assert(operator bool());
    assert(GetComponent<T>()==nullptr);
    return scene->AddComponent<T>(id, args...);
}

template<typename T>
T* GameObject::AddReferenceComponent(const GameObject& referenceId) const {
    assert(operator bool());
    return scene->AddReferenceComponent<T>(id, referenceId.id);
}

template<typename T>
T* GameObject::GetComponent() const {
    assert(operator bool());
    return scene->database.GetComponent<T>(id);
}

template<typename T>
void GameObject::RemoveComponent() const {
    assert(operator bool());
    assert(GetComponent<T>()!=nullptr);
    scene->RemoveComponent<T>(id);
}

template<typename Func>
void GameObject::IterateComponents(Func&& func) const {
    assert(operator bool());
    scene->database.IterateComponents(id, func);
}

template<typename Func>
void GameObject::IterateComponentsWithIndex(Func&& func) const {
    assert(operator bool());
    scene->database.IterateComponentsWithIndex(id, func);
}

template<typename...T>
void System<T...>::InitializeComponents(Scene& scene, ComponentSystemLists& componentSystemLists) {
    Scene** scenePtr = ((Scene**)&this->scene);
    *(scenePtr) = &scene;
    components = std::make_unique<Components>((scene.GetDatabase().AssureComponent<T>(), System<T...>::GetPointer<T>())...);
    PopulateComponentSystemLists(componentSystemLists);
}
  
}
}
