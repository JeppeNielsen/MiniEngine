//
//  AssetDatabase.hpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 04/07/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "ECS.hpp"
#include "FileSystemWatcher.hpp"
#include <unordered_map>
#include "LoadedAsset.hpp"
#include "Event.hpp"

namespace Mini {
    class AssetDatabase {
    public:
        AssetDatabase(const std::string& root, ECS::Database& database);
        ~AssetDatabase();
        
        std::string GuidToFullPath(const std::string& guid);
        std::string PathToGuid(const std::string& localPath);
        bool AssetExists(const std::string& localPath);
        bool CreateAsset(ECS::GameObject source, const std::string& localPath);
        ECS::GameObject LoadAsset(const std::string& guid);
        
        Event<> Changed;
        
    private:
    
        using GuidToPathMap = std::unordered_map<std::string, std::string>;
        using PathToGuidMap = std::unordered_map<std::string, std::string>;
        using LoadedAssets = std::unordered_map<std::string, LoadedAsset>;
    
        std::string root;
        ECS::Database& database;
        ECS::Scene scene;
        FileSystemWatcher fileWatcher;
        GuidToPathMap guidToPath;
        PathToGuidMap pathToGuid;
        LoadedAssets loadedAssets;
        
        std::string GetFullPath(std::string localPath);
        void AnyFileChanged();
        void ParseAll();
        std::string ParseGuidFromPath(const std::string& path);
        std::string CreateOrGetGuidFromPath(const std::string& fullPath);
    };
}
