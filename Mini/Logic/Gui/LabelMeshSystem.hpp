//
//  DynamicLabelSystem.hpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Label.hpp"
#include "Font.hpp"
#include <set>

namespace Mini {
    using namespace ECS;
    struct LabelMeshSystem : System<Label, Font, Mesh, Sizeable> {
    public:
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void SomethingChanged(GameObject object);
        void AddText(Mesh& mesh, const Font &font, std::string text, const Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Color& color);
        
        void Update(float dt);
        
        void TextChanged(GameObject object);
        
        void UpdateDirtyObject(GameObject object);
        
        std::set<GameObject> dirtyObjects;
    };
}
