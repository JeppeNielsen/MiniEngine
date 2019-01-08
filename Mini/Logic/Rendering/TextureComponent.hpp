//
//  TextureComponent.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Texture.hpp"

namespace Mini {
    struct TextureComponent {
    public:
        TextureComponent();
        ~TextureComponent();
        
        Mini::Texture& Texture();
        
        void LoadAsset(const std::string& path);
    private:
        class Mini::Texture texture;
        std::string prevPath;
    };
}
