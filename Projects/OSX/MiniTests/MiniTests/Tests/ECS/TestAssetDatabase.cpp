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

const std::string assetPath = "/Projects/MiniEngine/Projects/OSX/MiniTests/MiniTests/Tests/Assets";

void TestAssetDatabase::Run() {
    
    RunTest("Asset database ctor",[]() {
        
        Database database;
        AssetDatabase assetDatabase(assetPath, database);
        
        return true;
    });
    
    /*
    RunTest("Create asset",[]() {
        
        Database database;
        AssetDatabase assetDatabase(assetPath, database);
        
        Scene scene(database);
        auto object = scene.CreateObject();
        
        assetDatabase.CreateAsset(object, "Button.json");
        
        return FileHelper::FileExists(assetPath+"/" + "Button.json");
    });
    */
    
    RunTest("LoadAsset",[]() {
        
        Database database;
        database.AssureComponent<Position>();
        AssetDatabase assetDatabase(assetPath, database);
        
        auto gameObject = assetDatabase.LoadAsset("Mj0OtpKUTrC65nMgYPriHg==");
        
        auto pos = gameObject.GetComponent<Position>();
        
        std::cout << pos->x << std::endl;
        std::cout << pos->y << std::endl;
        
        return (bool)gameObject;
    });
}
