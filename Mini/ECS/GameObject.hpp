//
//  GameObject.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObjectId.hpp"
#include <vector>

namespace Mini {
namespace ECS {

class Scene;
class Hierarchy;

class GameObject {
public:
    GameObject();
    GameObject(Scene& scene, const GameObjectId id);
    GameObject(void* null);
    ~GameObject() = default;
    GameObject(GameObject&&) = default;
    GameObject(GameObject&) = default;
    GameObject(const GameObject&) = default;

    GameObject & operator=(const GameObject &) = default;
    GameObject & operator=(GameObject &&) = default;
    
    bool operator == (const GameObject& other) const;
    bool operator != (const GameObject& other) const;
    bool operator < (const GameObject& other) const;
    
    explicit operator bool() const;
    
    template<typename T, typename...Args>
    T* AddComponent(Args&&... args) const;
    
    template<typename T>
    T* AddReferenceComponent(const GameObject& reference) const;
    
    template<typename T>
    T* GetComponent() const;
    
    template<typename T>
    void RemoveComponent() const;
    
    template<typename Func>
    void IterateComponents(Func&& func) const;
    
    template<typename Func>
    void IterateComponentsWithIndex(Func&& func) const;
    
    void Remove() const;
    
    Hierarchy& Hierarchy() const;
    GameObjectId Id() const;
    Scene& Scene() const;
    
    void* AddComponent(int componentId) const;
    void* GetComponent(int componentId) const;
    void RemoveComponent(int componentId) const;
private:
    class Scene* scene;
    GameObjectId id;
};

using ObjectCollection = std::vector<GameObject>;

}
}
