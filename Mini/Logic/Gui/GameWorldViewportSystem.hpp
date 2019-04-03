//
//  GameWorldViewportSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "GameWorldViewport.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include "Mesh.hpp"
#include "TextureComponent.hpp"
#include "Touchable.hpp"

namespace Mini {
    class GameWorldViewportSystem : System<GameWorldViewport, Transform, Sizeable, Mesh, TextureComponent, Touchable> {
    protected:
        void ObjectAdded(GameObject object) override;
        void ObjectRemoved(GameObject object) override;
        void Update(float dt) override;
    public:
        void Render();
    private:
        void UpdateObject(GameObject object, float dt);
        void RenderObject(GameObject object);
        void TouchDown(TouchData d, GameObject object);
        void TouchUp(TouchData d, GameObject object);
    };
}
