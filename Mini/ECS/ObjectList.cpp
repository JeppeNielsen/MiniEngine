//
//  ObjectList.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "ObjectList.hpp"

using namespace Mini::ECS;

void ObjectList::Add(const GameObjectId object) {
    const auto objectIndex = object & GameObjectIdIndexMask;
    if (objectIndex>=indicies.size()) {
        indicies.resize(objectIndex + 1, GameObjectIdNull);
    }
    indicies[objectIndex] = objects.size();
    objects.emplace_back(object);
}

void ObjectList::Remove(const GameObjectId object) {
    const auto objectIndex = object & GameObjectIdIndexMask;
    const auto last = objects.back();
    const auto lastIndex = last & GameObjectIdIndexMask;
    
    const auto indexInObjects = indicies[objectIndex];
    const auto lastIndexInObjects = indicies[lastIndex];
    
    indicies[lastIndex] = indicies[objectIndex];
    indicies[objectIndex] = GameObjectIdNull;
    
    objects[indexInObjects] = objects[lastIndexInObjects];
    objects.pop_back();
}

bool ObjectList::Contains(const GameObjectId object) const {
    const auto index = object & GameObjectIdIndexMask;
    return index<indicies.size() && indicies[index] != GameObjectIdNull;
}

