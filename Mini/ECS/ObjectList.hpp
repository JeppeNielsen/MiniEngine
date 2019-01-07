//
//  ObjectList.hpp
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
  
class ObjectList {
public:
    using Objects = std::vector<GameObjectId>;
    using Indicies = std::vector<std::size_t>;

    void Add(const GameObjectId object);
    void Remove(const GameObjectId object);
    bool Contains(const GameObjectId object) const;

    Indicies indicies;
    Objects objects;
};

}
}
