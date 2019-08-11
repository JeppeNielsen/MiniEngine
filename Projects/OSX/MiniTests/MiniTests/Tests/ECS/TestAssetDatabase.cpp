//
//  TestAssetDatabase.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 04/07/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "TestAssetDatabase.hpp"
#include "AssetDatabase.hpp"
#include "FileHelper.hpp"
#include <chrono>
#include <thread>

using namespace Mini;
using namespace Tests::ECS;
using namespace Mini::ECS;

struct Position {
    int x;
    int y;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

const std::string assetsPath = "/Projects/MiniEngine/Projects/OSX/MiniTests/MiniTests/Tests/Assets";

void TestAssetDatabase::Run() {
    
    RunTest("Asset database ctor",[]() {
        
        Database database;
        AssetDatabase assetDatabase(assetsPath, database);
        
        return true;
    });
    
    
    RunTest("Create asset",[]() {
        
        Database database;
        AssetDatabase assetDatabase(assetsPath, database);
        
        Scene scene(database);
        auto object = scene.CreateObject();
        object.AddComponent<Position>(123,456);
        
        assetDatabase.CreateAsset(object, "Button.json");
        
        return assetDatabase.AssetExists("Button.json");
    });
    
    RunTest("Recreate asset, guid remains unchanged",[]() {
        
        Database database;
        AssetDatabase assetDatabase(assetsPath, database);
        
        auto guidBefore = assetDatabase.PathToGuid("Button.json");
        
        Scene scene(database);
        auto object = scene.CreateObject();
        object.AddComponent<Position>(123,456);
        
        assetDatabase.CreateAsset(object, "Button.json");
        
        auto guidAfter = assetDatabase.PathToGuid("Button.json");
        
        return guidBefore == guidAfter;
    });
    
    RunTest("LoadAsset",[]() {
        
        Database database;
        database.AssureComponent<Position>();
        AssetDatabase assetDatabase(assetsPath, database);
        
        std::string guid = assetDatabase.PathToGuid("Button.json");
        
        auto gameObject = assetDatabase.LoadAsset(guid);
        
        auto pos = gameObject.GetComponent<Position>();
        
        return (bool)gameObject && pos->x == 123 && pos->y == 456;
    });
    
    
    /*
    RunTest("Rename file, guid stays",[]() {
        
        using namespace std::chrono_literals;
        
        bool hasChanged = false;
        
        Database database;
        database.AssureComponent<Position>();
        AssetDatabase assetDatabase(assetsPath, database);
        assetDatabase.Changed.Bind([&](){ hasChanged = true; });
        
        
        std::string guid = assetDatabase.PathToGuid("Button.json");
        
        std::this_thread::sleep_for(5s);
        
        FileHelper::RenameFile(assetsPath+"/Button.json", assetsPath+"/ButtonNew.json");
        
        
        while (!hasChanged) {
            std::this_thread::sleep_for(1ms);
        }
        
        std::string pathNew = assetDatabase.GuidToFullPath(guid);
        return pathNew == (assetsPath + "/ButtonNew.json");
    });
    */
}
