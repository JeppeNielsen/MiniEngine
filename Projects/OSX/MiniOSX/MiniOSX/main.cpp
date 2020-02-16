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

struct AnimateToPosition {
    Vector3 target;
    Vector3 source;
    float t;
    float speed;
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
        go.GetComponent<Touchable>()->Click.Bind(this, &ClickRotationChanger::TouchDown, go);
    }
    
    void ObjectRemoved(GameObject go) {
        go.GetComponent<Touchable>()->Click.Unbind(this, &ClickRotationChanger::TouchDown, go);
    }
    
    void TouchDown(TouchData d, GameObject go) {
        go.GetComponent<RotationSpeed>()->speed = -go.GetComponent<RotationSpeed>()->speed;
        
       // JsonSerializer ser;
       // ser.SerializeObject(go, std::cout);
        
    }
};

struct ColorPulsatorSystem : System<ColorPulsator, Mesh> {
    void Update(float dt) {
        for(auto go : Objects()) {
            auto mesh = go.GetComponent<Mesh>();
            auto pulsator = go.GetComponent<ColorPulsator>();
            mesh->GetMesh<Vertex>().SetColor(Color::White(0.7f + sinf(pulsator->alpha) * 0.3f));
            pulsator->alpha += pulsator->speed * dt;
        }
    }
};

struct AnimateToSystem : System<Transform, AnimateToPosition> {

    void ObjectAdded(GameObject go) override {
        auto animateToPosition = go.GetComponent<AnimateToPosition>();
        animateToPosition->source = go.GetComponent<Transform>()->Position;
        animateToPosition->t = 0.0f;
    }

    void ObjectRemoved(GameObject go) override {
        std::cout << "Object removed";
    }

    void Update(float dt) override {
        for(auto go : Objects()) {
            auto animateToPosition = go.GetComponent<AnimateToPosition>();
            auto transform = go.GetComponent<Transform>();
            
            transform->Position = Vector3::Lerp(animateToPosition->source, animateToPosition->target, animateToPosition->t);
            animateToPosition->t += animateToPosition->speed * dt;
            if (animateToPosition->t>1.0f) {
                go.RemoveComponent<AnimateToPosition>();
                transform->Position = animateToPosition->target;
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
        
        auto& cameras = scene.CreateSystem<CameraSystem>();
        
        renderSystem = &scene.CreateSystem<RenderSystem>(scene.CreateSystem<RenderSystem::OctreeSystem>(), cameras);
        renderSystem->DefaultShader = &renderSystem->Shaders.LitColored;
        
        scene.CreateSystem<TouchSystem>(scene.CreateSystem<TouchSystem::OctreeSystem>(), cameras, device.Input);
        scene.CreateSystem<ClickRotationChanger>();
        scene.CreateSystem<FirstPersonMoverSystem>(device.Input);
        scene.CreateSystem<ColorPulsatorSystem>();
        scene.CreateSystem<RotationSpeedSystem>();
        scene.CreateSystem<AnimateToSystem>();
        
        auto camera = scene.CreateObject();
        camera.AddComponent<Camera>();
        camera.AddComponent<Transform>();
        camera.AddComponent<FirstPersonMover>();
        
        for(int i =0; i<1000; i++) {
            CreateCube({-10 + i * 4.0f, 0.0f, -10.0f},[](TouchData d) {
                
                if (d.object.GetComponent<AnimateToPosition>()) {
                    return;
                }
                auto a = d.object.AddComponent<AnimateToPosition>();
                a->target = d.object.GetComponent<Transform>()->Position() + Vector3(0,20,0);
                a->speed = 4.0f;
                
            });
        }
        
        device.Input.ButtonDown.Bind(this, &Game::ButtonDown);
        device.Screen.Size.Changed.Bind(this, &Game::ScreenSizeChanged);
    }
    
    void CreateCube(Vector3 pos, std::function<void(TouchData)> clicked) {
        auto cube = scene.CreateObject();
        cube.AddComponent<Transform>()->Position = pos;
        cube.AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube.GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Color::White(0.2f));
        cube.AddComponent<RotationSpeed>(Vector3(1,1,0));
        cube.AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        cube.AddComponent<Touchable>()->Click.Bind(clicked);
        cube.AddComponent<ColorPulsator>(pos.x, 3.0f);
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
