//
//  LoadedAsset.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 19/07/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "LoadedAsset.hpp"
#include "JsonDeserializer.hpp"
#include <fstream>

using namespace Mini;
using namespace ECS;

LoadedAsset::LoadedAsset(ECS::Scene& scene) : scene(scene) {}// : scene(Scene(database)) {}

void LoadedAsset::Load(const std::string& path) {
    JsonDeserializer deserializer;
    asset = scene.CreateObject();
    std::ifstream file;
    file.open(path.c_str());
    deserializer.DeserializeObject(asset, file);
    file.close();
}

GameObject LoadedAsset::Asset() {
    return asset;
}
