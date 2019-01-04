//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 03/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include <iostream>

#include "Engine.hpp"


struct Game : Mini::IState {
    void Initialize() override {
        std::cout << "Started" <<std::endl;
        
    }
    
    float time = 0;

    void Update(float dt) override {
        std::cout << time << std::endl;
        time += dt;
    }

    void Render() override {
        std::cout << "Render" << std::endl;
    }
};

int main() {
    Mini::Engine e;
    e.Start<Game>();
    return 0;
}
