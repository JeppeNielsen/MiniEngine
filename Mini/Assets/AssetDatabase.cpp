//
//  AssetDatabase.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 04/07/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "AssetDatabase.hpp"
#include "JsonSerializer.hpp"
#include "StringHelper.hpp"
#include "FileHelper.hpp"
#include <fstream>

using namespace Mini;
using namespace ECS;

AssetDatabase::AssetDatabase(const std::string& root, Database& database)
: root(root), database(database), scene(database) {
    fileWatcher.Start(root); fileWatcher.Changed.Bind(this, &AssetDatabase::AnyFileChanged);
    ParseAll();
}

AssetDatabase::~AssetDatabase() {
    fileWatcher.Stop();
}

std::string AssetDatabase::GuidToFullPath(const std::string& guid) {
    auto path = guidToPath.find(guid);
    return path != guidToPath.end() ? path->second : "";
}

std::string AssetDatabase::PathToGuid(const std::string& localPath) {
    auto fullPath = GetFullPath(localPath);
    auto guid = pathToGuid.find(fullPath);
    return guid != pathToGuid.end() ? guid->second : "";
}

bool AssetDatabase::AssetExists(const std::string &localPath) {
    auto fullPath = GetFullPath(localPath);
    return FileHelper::FileExists(fullPath);
}

bool AssetDatabase::CreateAsset(ECS::GameObject source, const std::string &path) {
    auto fullPath = GetFullPath(path);
    std::ofstream file;
    file.open(fullPath.c_str());
    if (!file.good()) return false;
    
    auto guid = CreateOrGetGuidFromPath(fullPath);
    JsonSerializer serializer([&guid](auto& writer) {
        writer.write("id", guid);
    });
    serializer.SerializeObject(source, file);
    file.close();
    guidToPath[guid] = fullPath;
    pathToGuid[fullPath] = guid;
    return true;
}

GameObject AssetDatabase::LoadAsset(const std::string &guid) {
    auto loadedAssetIt = loadedAssets.find(guid);
    if (loadedAssetIt != loadedAssets.end()) {
        return loadedAssetIt->second.Asset();
    }
    
    auto path = guidToPath.find(guid);
    if (path == guidToPath.end()) return nullptr;
    
    LoadedAsset loadedAsset(scene);
    loadedAsset.Load(path->second);
    loadedAssets.emplace(guid, std::move(loadedAsset));
    return loadedAsset.Asset();
}

std::string AssetDatabase::GetFullPath(std::string localPath) {
    return root + "/" + localPath;
}

void AssetDatabase::AnyFileChanged() {
    std::cout << "Any file changed" << std::endl;
    ParseAll();
    
    Changed();
}

void AssetDatabase::ParseAll() {
    guidToPath.clear();
    pathToGuid.clear();
    FileHelper::RecurseFolder(root, [this] (auto fullPath) {
        //std::cout << path<<std::endl;
        auto guid = ParseGuidFromPath(fullPath);
        if (guid == "") return;
        guidToPath.emplace(guid, fullPath);
        pathToGuid.emplace(fullPath, guid);
    });
}

std::string AssetDatabase::ParseGuidFromPath(const std::string& path) {
    std::ifstream file;
    file.open(path.c_str());
    
    minijson::istream_context context(file);
    try {
        std::string foundGuid = "";
        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string token = n;
            if (v.type() == minijson::String && token == "id") {
                foundGuid = v.as_string();
                minijson::ignore(context);
            }
            minijson::ignore(context);
        });
        return foundGuid;
    } catch (minijson::parse_error e) {
        return "";
    }
}

std::string AssetDatabase::CreateOrGetGuidFromPath(const std::string &fullPath) {
    auto it = pathToGuid.find(fullPath);
    return it != pathToGuid.end() ? it->second : StringHelper::CreateGuid();
}
