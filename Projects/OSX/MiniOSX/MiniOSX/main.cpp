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

struct RotationSpeed {
    Vector3 speed;
};

struct RotationSpeedSystem : System<Transform, RotationSpeed> {
    void Update(float dt) {
        for(auto go : Objects()) {
            go.GetComponent<Transform>()->EulerRotation += go.GetComponent<RotationSpeed>()->speed * dt;
        }
    }
};

struct Game : IState {

    Database database;
    Scene scene;
    RenderSystem* renderSystem;
    
    Game() : scene(database) { }

    void Initialize() override {
    
        renderSystem = &scene.CreateSystem<RenderSystem>();
        scene.CreateSystem<RotationSpeedSystem>();
        
        auto camera = scene.CreateObject();
        camera.AddComponent<Camera>();
        camera.AddComponent<Transform>();

        auto cube = scene.CreateObject();
        cube.AddComponent<Transform>()->Position = {0,0,-10};
        cube.AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube.AddComponent<RotationSpeed>(Vector3(1,1,0));
        cube.AddComponent<Renderable>();
        renderSystem->DefaultShader = &renderSystem->Shaders.LitColored;
        
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
        device.Screen.Size.Changed.Bind(this, &Game::ScreenSizeChanged);
    }
    
    void ButtonDown(ButtonEvent e) {
        std::cout << e.Id << std::endl;
        if (e.Id == "j") device.Exit();
    }
    
    void ScreenSizeChanged() {
        std::cout << "Screen size changed: " << device.Screen.Size << std::endl;
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
