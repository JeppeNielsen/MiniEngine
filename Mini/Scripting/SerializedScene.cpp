//
//  SerializedScene.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 01/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "SerializedScene.hpp"
#include "JsonSerializer.hpp"
#include "JsonDeserializer.hpp"
#include <sstream>

using namespace Mini;

void SerializedScene::Serialize(ECS::Scene &scene, const std::function<bool (int)> &componentFilter) {
    data.clear();
    for(auto go : scene.Objects()) {
        std::stringstream ss;
        JsonSerializer serializer;
        serializer.SerializeComponents(go, ss, componentFilter);
        data[go.Id()] = ss.str();
    }
}

void SerializedScene::Deserialize(ECS::Scene &scene) const {
    for(auto go : scene.Objects()) {
        auto it = data.find(go.Id());
        if (it == data.end()) continue;
        JsonDeserializer deserializer;
        std::stringstream ss;
        ss<<it->second;
        deserializer.DeserializeComponents(go, ss);
    }
}
