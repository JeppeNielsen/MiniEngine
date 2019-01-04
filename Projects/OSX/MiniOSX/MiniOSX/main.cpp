//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include <iostream>

#include "Engine.hpp"

using namespace Mini;

struct Game : IState {
    void Initialize() override {
        std::cout << "Started" <<std::endl;
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
        device.Input.TouchDown.Bind(this, &Game::TouchDown);
        device.Input.TouchUp.Bind(this, &Game::TouchUp);
    }
    
    void TouchDown(TouchEvent e) {
        std::cout << "Touch down : " << e.Position << std::endl;
    }
    
    void TouchUp(TouchEvent e) {
        std::cout << "Touch Up : " << e.Position << std::endl;
    }
    
    void ButtonDown(ButtonEvent e) {
        std::cout << "Button  : " << e.Id << std::endl;
    }
    
    void Update(float dt) override {
    }

    void Render() override {
    }
};

int main() {
    Mini::Engine e;
    e.Start<Game>();
    return 0;
}
