//
//  LoadedAsset.hpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 19/07/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "ECS.hpp"

namespace Mini {
    class LoadedAsset {
    public:
        LoadedAsset(ECS::Scene& scene);
        ~LoadedAsset() = default;
        LoadedAsset(LoadedAsset&& other) = default;
        
        void Load(const std::string& path);
        ECS::GameObject Asset();
    private:
        ECS::Scene& scene;
        ECS::GameObject asset;
    };
}

