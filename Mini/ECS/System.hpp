//
//  System.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <memory>
#include "ObjectList.hpp"
#include "Container.hpp"
#include "Hierarchy.hpp"
#include "GameObject.hpp"
#include "MetaHelper.hpp"
#include "GameObjectIterator.hpp"
#include "Database.hpp"

namespace Mini {
namespace ECS {

class ISystem {
public:
    using ComponentSystemLists = std::vector<std::vector<ISystem*>>;

    virtual ~ISystem() = default;
    virtual void Initialize() = 0;
    virtual void InitializeComponents(Scene* scene, ComponentSystemLists& systemsUsingComponent) = 0;
    virtual void TryAddObject(const GameObjectId object) = 0;
    virtual void RemoveObject(const GameObjectId object) = 0;
    virtual void Update(float dt) = 0;
    ObjectList objects;
};

template<typename...T>
class System : public ISystem {
public:
    using Components = std::tuple<T*...>;
    
    void Initialize() override { }

    void InitializeComponents(Scene& scene, ComponentSystemLists& componentSystemLists);
    
    void InitializeComponents(Scene* scene, ComponentSystemLists& componentSystemLists) override {
        Scene** scenePtr = ((Scene**)&this->scene);
        *(scenePtr) = scene;
        components = std::make_unique<Components>(GetPointer<T>()...);
        PopulateComponentSystemLists(componentSystemLists);
    }
    
    void PopulateComponentSystemLists(ComponentSystemLists& componentSystemLists) {
        Meta::for_each(*components, [&componentSystemLists, this](auto component) {
           using componentType = std::remove_pointer_t<decltype(component)>;
           const auto componentId = Database::IdHelper::GetId<componentType>();
           if (componentId>=componentSystemLists.size()) {
               componentSystemLists.resize(componentId + 1);
           }
           componentSystemLists[componentId].push_back(this);
        });
    }

    bool Match(const GameObjectId objectId) const {
        bool contains = true;
        GameObject object(*scene, objectId);
        Meta::for_each(*components, [&contains, &object](auto component) {
           using componentType = std::remove_pointer_t<decltype(component)>;
           contains &= object.GetComponent<componentType>()!=nullptr;
        });
        return contains;
    }
    
    void TryAddObject(const GameObjectId object) override {
        if (!Match(object)) return;
        objects.Add(object);
        ObjectAdded(GameObject(*scene, object));
    }
    
    void RemoveObject(const GameObjectId object) override {
        ObjectRemoved(GameObject(*scene, object));
        objects.Remove(object);
    }
    
    const GameObjectIterator Objects() {
        return GameObjectIterator(*scene, objects.objects, 0);
    }
    
protected:
    virtual void ObjectAdded(GameObject object) {}
    virtual void ObjectRemoved(GameObject object) {}
    virtual void Update(float dt) override {}
    Scene* const scene = nullptr;
private:
    template<typename TPointer> TPointer* GetPointer() { return nullptr; }
    std::unique_ptr<Components> components;
};

}
}
