//
//  Database.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObjectId.hpp"
#include "Container.hpp"
#include "IdHelper.hpp"
#include "ClassNameHelper.hpp"

namespace Mini {
namespace ECS {

class Scene;
class GameObject;

class Database {
public:
    using IdHelper = IdHelper<struct ComponentId>;
private:
    using ComponentsIndexed = std::vector<std::unique_ptr<IContainer>>;
    using Components = std::vector<IContainer*>;
    using ComponentNames = std::vector<std::string>;
    
public:
    template<typename T>
    void AssureComponent() {
        const auto id = IdHelper::GetId<T>();
        if (id>=componentsIndexed.size()) {
            componentsIndexed.resize(id + 1);
        }
        if (componentsIndexed[id]) return;
        componentsIndexed[id] = std::make_unique<Container<T>>();
        components.push_back(componentsIndexed[id].get());
        componentNames.push_back(ClassNameHelper::GetName<T>());
    }
    
    void AddCustomComponent(int id, IContainer* container, const std::string& name);
    
    void RemoveCustomComponent(int id);
    
    template<typename T>
    Container<T>& ComponentContainer() const {
        const auto id = IdHelper::GetId<T>();
        return static_cast<Container<T>&>(*componentsIndexed[id]);
    }
    
    bool TryFindComponentContainer(const std::string& componentName, IContainer** container, int& componentId);
    
    using NameIndex = std::pair<std::string, int>;
    
    std::vector<NameIndex> GetComponentNameIndices();
    
private:

    template<typename T, typename... Args>
    T* AddComponent(const GameObjectId objectId, Args&& ... args) {
        AssureComponent<T>();
        ComponentContainer<T>().Create(objectId, args...);
        return ComponentContainer<T>().Get(objectId);
    }
    
    template<typename T>
    T* AddReferenceComponent(const GameObjectId objectId, const GameObjectId referenceId) {
        AssureComponent<T>();
        ComponentContainer<T>().Reference(objectId, referenceId);
        return ComponentContainer<T>().Get(objectId);
    }
    
    template<typename T>
    T* GetComponent(const GameObjectId objectId) const {
        const auto componentId = IdHelper::GetId<T>();
        if (componentId>=componentsIndexed.size() || !componentsIndexed[componentId]) return nullptr;
        auto& container = ComponentContainer<T>();
        return container.Contains(objectId) ? container.Get(objectId) : nullptr;
    }
    
    template<typename Func>
    void IterateComponents(const GameObjectId objectId, Func&& func) const {
        for(auto& component : components) {
            if (!component->Contains(objectId)) continue;
            func(component);
        }
    }
    
    template<typename Func>
    void IterateComponentsWithIndex(const GameObjectId objectId, Func&& func) const {
        for (size_t i=0; i<componentsIndexed.size(); ++i) {
            if (!componentsIndexed[i]) continue;
            if (!componentsIndexed[i]->Contains(objectId)) continue;
            func(componentsIndexed[i].get(), (int)i);
        }
    }
    
    TypeInfo GetTypeInfo(const GameObjectId objectId, const std::size_t componentId);
    
    void RemoveComponent(const GameObjectId objectId, const std::size_t componentId);
    
    void RemoveAllComponents(const GameObjectId objectId);
    
    bool GameObjectIdValid(const GameObjectId object) const;
    
    GameObjectId CreateObject();
    
    void Remove(const GameObjectId objectId);
    
    friend class Scene;
    friend class GameObject;
    
    ComponentsIndexed componentsIndexed;
    Components components;
    ComponentNames componentNames;
    
    std::vector<GameObjectId> objects;
    std::size_t available {};
    GameObjectId next {};
};

}
}
