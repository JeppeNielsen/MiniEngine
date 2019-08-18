//
//  ScriptWorld.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 28/12/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include "ECS.hpp"
#include "SystemComponentExtractor.hpp"
#include "SerializedScene.hpp"

namespace cling {
    class Interpreter;
}

namespace Mini {
    class ScriptWorld {
    public:
        ~ScriptWorld();
    
        void Initialize(const std::string& clangPath, const std::vector<std::string>& includePaths);
        bool Compile(ECS::Database& database, const std::vector<std::string>& cppFiles);
        void RemoveFromDatabase();
        void Clear();
        void AddScene(ECS::Scene& scene, const SerializedScene& serializedScene);
        SerializedScene RemoveScene(ECS::Scene& scene);
        
    private:
    
        using ComponentNameIndicies = std::vector<ECS::Database::NameIndex>;
    
        std::string Code_ForwardDeclareComponents(const ComponentNameIndicies& componentNameIndices);
        std::string Code_AddGetRemoveComponent(const ComponentNameIndicies& componentNameIndices);
        std::string Code_CreateComponent(const ComponentNameIndicies& componentNameIndices);
        std::string Code_RemoveComponent(const ComponentNameIndicies& componentNameIndices);
        std::string Code_GetTypeInfo(SystemComponentExtractor::Components components, int startScriptComponentIndex);
        std::string Code_CreateSystem(SystemComponentExtractor::Systems systems);
        std::string Code_RemoveSystem(SystemComponentExtractor::Systems systems);
        
        std::string clangPath;
        std::vector<std::string> includePaths;
        cling::Interpreter* interpreter = nullptr;
        ECS::Database* database = nullptr;
        std::vector<int> createdComponentIds;
        int startComponentIndex;
        int endComponentIndex;
    };
}
