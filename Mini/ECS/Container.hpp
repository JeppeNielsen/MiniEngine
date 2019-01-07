//
//  Container.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameObjectId.hpp"
#include <vector>
#include <deque>
#include <assert.h>
#include "MetaHelper.hpp"
#include "TypeInfo.hpp"

namespace Mini {
namespace ECS {

struct IContainer {
    virtual ~IContainer() = default;
    
    bool Contains(const GameObjectId id) const {
        const auto index = id & GameObjectIdIndexMask;
        return index<indicies.size() && indicies[index] != GameObjectIdNull;
    }
    
    virtual void CreateDefault(const GameObjectId id) = 0;
    
    virtual void Destroy(const GameObjectId id) = 0;
    virtual void* GetInstance(const GameObjectId id) = 0;
    
    virtual void Serialize(const GameObjectId id, minijson::object_writer& writer) = 0;
    virtual void Deserialize(const GameObjectId id, minijson::istream_context& context) = 0;
    
    std::vector<std::uint32_t> indicies;
};

template<typename T>
struct Container : public IContainer {

    Container() = default;
    virtual ~Container() = default;

    Container(const Container &) = delete;
    Container(Container &&) = default;

    Container & operator=(const Container &) = delete;
    Container & operator=(Container &&) = default;
    
    void CreateIndex(const GameObjectId id) {
        assert(!Contains(id));
        const auto index = id & GameObjectIdIndexMask;
        if (index>=indicies.size()) {
            indicies.resize(index + 1, GameObjectIdNull);
        }
        indicies[index] = (std::uint32_t)elements.size();
        references.emplace_back(1);
    }
    
    void CreateDefault(const GameObjectId id) override {
        CreateIndex(id);
        elements.emplace_back(T{});
    }
    
    template<typename... Args>
    std::enable_if_t<!std::is_constructible<T, Args...>::value, void>
    Create(const GameObjectId id, Args&& ... args) {
        CreateIndex(id);
        elements.emplace_back(T{std::forward<Args>(args)...});
    }
    
    template<typename... Args>
    std::enable_if_t<std::is_constructible<T, Args...>::value, void>
    Create(const GameObjectId id, Args&& ... args) {
        CreateIndex(id);
        elements.emplace_back(std::forward<Args>(args)...);
    }
    
    void Reference(const GameObjectId id, const GameObjectId referenceId) {
        assert(!Contains(id));
        assert(Contains(referenceId));
        const auto index = id & GameObjectIdIndexMask;
        const auto referenceIndex = referenceId & GameObjectIdIndexMask;
        if (index>=indicies.size()) {
            indicies.resize(index + 1, GameObjectIdNull);
        }
        
        indicies[index] = referenceIndex;
        ++references[referenceIndex];
    }
    
    void Destroy(const GameObjectId id) override {
        assert(Contains(id));
        const auto index = id & GameObjectIdIndexMask;
        
        if ((references[index]--)==0) {
            auto tmp = std::move(elements.back());
            elements[index] = std::move(tmp);
            elements.pop_back();
            references[index] = references.back();
            references.pop_back();
            indicies[indicies.size() - 1] = index;
        }
        
        indicies[index] = GameObjectIdNull;
    }
    
    T* Get(const GameObjectId id) {
        const auto index = id & GameObjectIdIndexMask;
        return &elements[indicies[index]];
    }
    
    void* GetInstance(const GameObjectId id) override {
        if (!Contains(id)) return nullptr;
        return Get(id);
    }
    
    template<typename Type>
    static void TrySerializeFields(minijson::object_writer& writer, Type& instance) {
        Mini::Meta::static_if<HasGetTypeMethod<Type>, Type&>(instance, [&writer](auto& getType) {
            auto typeInfo = getType.GetType();
            auto name = writer.nested_object(typeInfo.Name().c_str());
            typeInfo.Serialize(name);
            name.close();
        });
    }
    
    template<typename Type>
    static void TryDeserializeFields(minijson::istream_context& context, Type& instance) {
        Mini::Meta::static_if<HasGetTypeMethod<Type>, Type&>(instance, [&context](auto& getType) {
            auto typeInfo = getType.GetType();
            typeInfo.Deserialize(context);
        });
    }
    
    virtual void Serialize(const GameObjectId id, minijson::object_writer& writer) override {
        T* instance = Get(id);
        TrySerializeFields(writer, *instance);
    }
    
    virtual void Deserialize(const GameObjectId id, minijson::istream_context& context) override {
        T* instance = Get(id);
        TryDeserializeFields(context, *instance);
    }
    
    
    std::deque<T> elements;
    std::vector<std::uint16_t> references;
};

}
}
