//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 06/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"

using namespace Mini;

struct Game : IState {

    void Initialize() override {
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
        device.ScreenSize.Changed.Bind(this, &Game::ScreenSizeChanged);
    }
    
    void ButtonDown(ButtonEvent e) {
        std::cout << e.Id << std::endl;
        if (e.Id == "j") device.Exit();
    }
    
    void ScreenSizeChanged() {
        std::cout << "Screen size changed: " << device.ScreenSize << std::endl;
    }

    void Update(float dt) override {
    
    }

    void Render() override {
    
    }
};

int main() {
    Engine e;
    e.Start<Game>();
    return 0;
}
