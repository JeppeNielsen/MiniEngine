//
//  main.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 06/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "ECS.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "JsonSerializer.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "Gui.hpp"
#include "JsonDeserializer.hpp"

using namespace Mini;
using namespace Mini::ECS;


struct Rotator {
    float rotation;
    float speed;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(rotation)
    TYPE_FIELD(speed)
    TYPE_FIELDS_END
};

struct RotatorSystem : System<Transform, Rotator> {
    void Update(float dt) {
        for(auto go : Objects()) {
            auto rotator = go.GetComponent<Rotator>();
            go.GetComponent<Transform>()->Rotation = Quaternion(Vector3(0,0,rotator->rotation));
            rotator->rotation += rotator->speed * dt;
        }
    }
};

struct TestGUI : IState {

    Database database;
    Scene scene;
    Gui gui;
    GameObject clone;
    bool set = false;
    
    TestGUI() : scene(database), gui(scene, device.Input) { }

    GameObject CreateControl(GameObject root, float speed) {
        auto control = gui.CreateControl(root, "", {100,100}, {200, 200});
        control.GetComponent<Colorable>()->Color = Color::Blue(0.95f);
        control.AddComponent<Draggable>();
        control.AddComponent<Rotator>(0.0f, -speed);
        
        auto child = gui.CreateControl(control, "", {0,0}, {200, 10});
        child.AddComponent<Rotator>(0.0f, speed);
        return control;
    }

    void Initialize() override {
    
        Transform t1;
        t1.Position = {1,2,3};
        
        Transform t2;
        
        t2 = t1;
        
        t1.Position.Changed.Bind([&]() {
            std::cout << t1.Position << std::endl;
        });
        t1.Position = {5,5,5};
        
        std::cout <<  t2.Local;
        
        
        device.Menu.AddChild("Menu").AddChild("Exit", "E").Clicked.Bind([this]() {
            device.Exit();
        });
        
        device.Input.ButtonDown.Bind([](auto e) {
            std::cout << e.Id << std::endl;
        });
        
        gui.CreateSystems();
        
        scene.CreateSystem<RotatorSystem>();
        
        auto atlasGO = scene.CreateObject();
        auto atlas = atlasGO.AddComponent<Atlas>();
        atlasGO.AddComponent<TextureComponent>();
        
        gui.Setup(atlasGO, {0, device.Screen.Size});
        
        auto root = scene.CreateObject();
        
        auto control = CreateControl(root, 3.0f);
        
        std::stringstream buffer;
        
        JsonSerializer save;
        save.SerializeObject(control, buffer);
        
        JsonDeserializer load;
        clone = scene.CreateObject();
        load.DeserializeObject(clone, buffer);
        
        std::cout << buffer.str() << std::endl;
        
    }
    
    void Update(float dt) override {
        scene.Update(dt);
    }

    void Render() override {
        gui.Render();
    }
};

int main() {
    Engine e;
    e.Start<TestGUI>();
    return 0;
}

