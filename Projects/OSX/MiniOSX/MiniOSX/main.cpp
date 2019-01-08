//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 06/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "Scene.hpp"
#include "RenderSystem.hpp"

using namespace Mini;
using namespace Mini::ECS;

struct Game : IState {

    Database database;
    Scene scene;
    RenderSystem* renderSystem;
    
    Game() : scene(database) { }

    void Initialize() override {
    
        renderSystem = &scene.CreateSystem<RenderSystem>();
        
        auto camera = scene.CreateObject();
        camera.AddComponent<Camera>();
        camera.AddComponent<Transform>();

        auto cube = scene.CreateObject();
        cube.AddComponent<Transform>()->Position = {0,0,-10};
        cube.AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube.GetComponent<Mesh>()->GetMesh<Vertex>().AddCube({2,0,0}, 1);
        
        cube.AddComponent<Renderable>();
        

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
        scene.Update(dt);
    }

    void Render() override {
        renderSystem->Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
    return 0;
}
