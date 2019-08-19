//
//  CoreECS.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestScripting.hpp"
#include "ScriptWorld.hpp"
#include "FileHelper.hpp"
#include "JsonSerializer.hpp"

using namespace Mini;
using namespace ECS;
using namespace Tests::ECS;

#define SCRIPT(...) #__VA_ARGS__

const std::string clangPath = "/Projects/MiniEngine/Mini/Scripting/clang";

const std::string tempScriptPath = "Script.cpp";

std::string WriteScript(const std::string& code) {
    FileHelper::CreateTextFile(tempScriptPath, code);
    return tempScriptPath;
}

void TestScripting::SetupScriptWorld(ScriptWorld& scriptWorld) {
    
    std::vector<std::string> includePaths = {
        "/Projects/MiniEngine/Mini/Tools/",
        "/Projects/MiniEngine/Mini/ECS/",
        "/Projects/MiniEngine/Mini/Reflection/",
        "/Projects/MiniEngine/Mini/Serialization/",
        "/Projects/MiniEngine/Mini/Json/",
        "/Projects/MiniEngine/Mini/Helpers/"
    };
    
    scriptWorld.Initialize(clangPath, includePaths);
}

void TestScripting::Run() {

    RunTest("ScriptWorld script", [this] () -> bool {
        ScriptWorld scriptWorld;
        SetupScriptWorld(scriptWorld);
        
        std::string header = "#include \"ECS.hpp\"\n";
        
        std::string script = header + SCRIPT(
        
            using namespace Mini::ECS;
            
            struct Position {
                float x;
                float y;
            };
            
            struct Velocity {
                float vx;
                float vy;
            };
            
            struct MovementSystem : System<Position, Velocity> {
            
                void ObjectAdded(GameObject go) override {
                    std::cout << "Object added" << std::endl;
                }
                
                void ObjectRemoved(GameObject go) override {
                    std::cout << "Object removed" << std::endl;
                }
            
                void Update(float dt) override {
                    for(auto o : Objects()) {
                        auto pos = o.GetComponent<Position>();
                        auto vel = o.GetComponent<Velocity>();
                        
                        pos->x += vel->vx;
                        pos->y += vel->vy;
                    }
                }
            };
        );
        
        Database database;
        database.AssureComponent<Hierarchy>();
        if (!scriptWorld.Compile(database, { WriteScript(script) })) {
            return false;
        }
        
        Scene scene(database);
        scriptWorld.AddScene(scene, {});
        
        auto go = scene.CreateObject();
        go.AddComponent(1);
        go.AddComponent(2);
        
        auto positionType = go.GetTypeInfo(2);
        FieldInfo info;
        positionType.TryFindField("vx", info);
        auto xpos = info.GetField<float>();
        *xpos = 1;
        
        JsonSerializer serializer;
        serializer.SerializeObject(go, std::cout);
        
        scene.Update(0.0f);
        
        serializer.SerializeObject(go, std::cout);
        
        go.RemoveComponent(2);
        
        scene.Update(0.0f);
        
        serializer.SerializeObject(go, std::cout);
        
        return true;
    });
    
}
