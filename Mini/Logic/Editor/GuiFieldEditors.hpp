//
//  SerializedFieldEditors.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TextBox.hpp"
#include "TypeInfo.hpp"
#include "Property.hpp"
#include <vector>
#include <functional>

namespace Pocket {
    class Gui;
    struct GuiFieldEditor : public IFieldEditor {
        static void Initialize();
    
        void Create(const std::string& name, void* context, void* parent, GameObject* object) override {
            Initialize(name, (Gui*)context, (GameObject*) parent, object);
        }
        virtual void Initialize(const std::string& name, Gui* gui, GameObject* guiParent, GameObject* fieldObject) = 0;
    };
}
