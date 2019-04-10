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
#include "TouchSystem.hpp"
#include "JsonSerializer.hpp"
#include "FirstPersonMoverSystem.hpp"

using namespace Mini;
using namespace Mini::ECS;

struct RotationSpeed {
    Vector3 speed;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(speed)
    TYPE_FIELDS_END
};

struct ColorPulsator {
    float alpha;
    float speed;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(alpha)
    TYPE_FIELD(speed)
    TYPE_FIELDS_END
};

struct RotationSpeedSystem : System<Transform, RotationSpeed> {
    void Update(float dt) {
        for(auto go : Objects()) {
            go.GetComponent<Transform>()->EulerRotation += go.GetComponent<RotationSpeed>()->speed * dt;
        }
    }
};

struct ClickRotationChanger : System<RotationSpeed, Touchable> {
    void ObjectAdded(GameObject go) {
        go.GetComponent<Touchable>()->Down.Bind(this, &ClickRotationChanger::TouchDown, go);
    }
    
    void ObjectRemoved(GameObject go) {
        go.GetComponent<Touchable>()->Down.Unbind(this, &ClickRotationChanger::TouchDown, go);
    }
    
    void TouchDown(TouchData d, GameObject go) {
        go.GetComponent<RotationSpeed>()->speed = -go.GetComponent<RotationSpeed>()->speed;
        
        JsonSerializer ser;
        ser.SerializeObject(go, std::cout);
        
    }
};

struct ColorPulsatorSystem : System<ColorPulsator, Mesh> {
    void Update(float dt) {
        for(auto go : Objects()) {
            auto mesh = go.GetComponent<Mesh>();
            auto pulsator = go.GetComponent<ColorPulsator>();
            mesh->GetMesh<Vertex>().SetColor(Color::White(pulsator->alpha));
            pulsator->alpha += pulsator->speed * dt;
            if (pulsator->alpha>1.0f) {
                pulsator->alpha = 0.0f;
            }
        }
    }
};

struct Game : IState {

    Database database;
    Scene scene;
    RenderSystem* renderSystem;
    
    Game() : scene(database) { }

    void Initialize() override {
    
        device.Menu.AddChild("First").AddChild("Sub").Clicked.Bind([this] () {
            device.Exit();
        });
        device.Menu.AddChild("Second").AddChild("Sub").AddChild("Sub 2").AddChild("Sub 3");
        
    
        renderSystem = &scene.CreateSystem<RenderSystem>();
        renderSystem->DefaultShader = &renderSystem->Shaders.LitColored;
        
        scene.CreateSystem<RotationSpeedSystem>();
        scene.CreateSystem<TouchSystem>().Input = &device.Input;
        scene.CreateSystem<ClickRotationChanger>();
        scene.CreateSystem<FirstPersonMoverSystem>().Input = &device.Input;
        scene.CreateSystem<ColorPulsatorSystem>();
        
        auto camera = scene.CreateObject();
        camera.AddComponent<Camera>();
        camera.AddComponent<Transform>();
        camera.AddComponent<FirstPersonMover>();
        
        for(int i =0; i<100; i++) {
            CreateCube({-10 + i * 4.0f, 0.0f, -10.0f});
        }
        
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
        device.Screen.Size.Changed.Bind(this, &Game::ScreenSizeChanged);
    }
    
    void CreateCube(Vector3 pos) {
        auto cube = scene.CreateObject();
        cube.AddComponent<Transform>()->Position = pos;
        cube.AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube.GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Color::White(0.2f));
        cube.AddComponent<RotationSpeed>(Vector3(1,1,0));
        cube.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        cube.AddComponent<Touchable>();
        cube.AddComponent<ColorPulsator>(pos.x, 0.3f);
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
