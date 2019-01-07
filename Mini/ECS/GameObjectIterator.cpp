//
//  GameObjectIterator.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 01/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectIterator.hpp"

using namespace Mini::ECS;

GameObjectIterator::GameObjectIterator(Scene& scene, ObjectList::Objects& objects, std::size_t index) : scene(scene), objects(objects), index(index) {}

GameObjectIterator GameObjectIterator::begin() const {
    return GameObjectIterator(scene, objects, 0);
}

GameObjectIterator GameObjectIterator::end() const {
    return GameObjectIterator(scene, objects, objects.size());
}

GameObjectIterator& GameObjectIterator::operator++() { ++index; return *this; }
bool GameObjectIterator::operator!=(const GameObjectIterator & other) const { return index != other.index; }
const GameObject GameObjectIterator::operator*() const { return GameObject(scene, objects[index]); }
