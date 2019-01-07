//
//  GameObjectIterator.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 01/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include "ObjectList.hpp"

namespace Mini {
namespace ECS {
    
    class Scene;
    
    class GameObjectIterator {
    public:
        GameObjectIterator(Scene& scene, ObjectList::Objects& objects, std::size_t index);
        GameObjectIterator(GameObjectIterator&&) = default;
        GameObjectIterator(GameObjectIterator&) = default;
        GameObjectIterator(const GameObjectIterator&) = default;
        
        GameObjectIterator begin() const;
        GameObjectIterator end() const;
        
        GameObjectIterator& operator++();
        bool operator!=(const GameObjectIterator & other) const;
        const GameObject operator*() const;
    
    private:
        size_t index;
        Scene& scene;
        std::vector<GameObjectId>& objects;
    };
}
}
