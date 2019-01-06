//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 06/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"

struct Game : Mini::IState {

    void Initialize() override {
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
    }
    
    void ButtonDown(Mini::ButtonEvent e) {
        std::cout << e.Id << std::endl;
        if (e.Id == "j") device.Exit();
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
