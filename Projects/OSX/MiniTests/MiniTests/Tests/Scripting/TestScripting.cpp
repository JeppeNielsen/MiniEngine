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

    RunTest("ScriptWorld ctor", [this] () -> bool {
        ScriptWorld scriptWorld;
        SetupScriptWorld(scriptWorld);
        
        std::string header = "#include \"ECS.hpp\"\n";
        
        std::string script = header + SCRIPT(
        
            using namespace Mini::ECS;
            
            struct Position {
                float x;
                float y;
            };
            
            struct MovementSystem : System<Position> {
                void Update(float dt) override {
                
                }
            };
        );
        
        Database database;
        scriptWorld.Compile(database, { WriteScript(script) });
        
        return true;
    });
    
}
