//
//  TextBoxSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "System.hpp"
#include "TextBox.hpp"
#include "Touchable.hpp"
#include <map>

namespace Mini {
    class Gui;
    class TextBoxSystem : System<TextBox, Touchable> {
    public:
        
        void Initialize();
        void ObjectAdded(GameObject object);
        void ObjectRemoved(GameObject object);
        void ActiveTextBoxChanged(GameObject object);
        void TouchInputUp(TouchEvent e);
        void TouchDown(TouchData d, GameObject object);
        void TouchUp(TouchData d, GameObject object);
        void TouchClick(TouchData d, GameObject object);
        void Update(float dt);
        void KeyboardActiveChanged();
        void KeyboardTextChanged();
        
    private:
        struct pushedTextBox {
            TouchData touch;
            Vector2 lastTouchPosition;
            float distanceMoved;
        };
        
        typedef std::map<GameObject, pushedTextBox> PushedTextBoxes;
        PushedTextBoxes pushedTextBoxes;
        
        bool touchWasUp;
        bool anyTextboxUp;

    public:
        Property<TextBox*> ActiveTextBox;
        
        friend class Gui;
    };
}
