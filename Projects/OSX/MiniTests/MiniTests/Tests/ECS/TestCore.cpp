//
//  CoreECS.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 22/09/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TestCore.hpp"
#include "ECS.hpp"

using namespace Mini::ECS;
using namespace Tests::ECS;

void TestCore::Run() {

    RunTest("GameObject handle", [] () -> bool {
        GameObject gameObject;
        return (bool)!gameObject;
    });
    
    RunTest("Create GameObject", [] () -> bool {
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        return (bool)gameObject;
    });
    
    RunTest("GameObject == operator", [] () -> bool {
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        GameObject gameObject2 = gameObject;
        return gameObject == gameObject2;
    });
    
    RunTest("Remove GameObject", [] () -> bool {
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        bool wasValid = (bool)gameObject;
        gameObject.Remove();
        scene.Update(0.0f);
        return (bool)!gameObject && wasValid;
    });
    
    RunTest("AddComponent<T>()->data = 123", [] () -> bool {
        struct Component { int data; };
        
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        auto component = gameObject.AddComponent<Component>();
        component->data = 123;
        return component->data == 123;
    });

    RunTest("AddComponent<T>(123)", [] () -> bool {
        struct Component { int data; };
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        auto component = gameObject.AddComponent<Component>(123);
        return component->data = 123;
    });

    RunTest("GetComponent<T>()", [] () -> bool {
        struct Component { int data; };
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        gameObject.AddComponent<Component>(123);
        return gameObject.GetComponent<Component>()->data == 123;
    });
    
    RunTest("RemoveComponent", [] () -> bool {
        struct Component { int data; };
        Database database;
        Scene scene(database);
        GameObject gameObject = scene.CreateObject();
        gameObject.AddComponent<Component>(123);
        gameObject.RemoveComponent<Component>();
        scene.Update(0.0f);
        return gameObject.GetComponent<Component>() == nullptr;
    });
    
    RunTest("AddReferenceComponent", [] () -> bool {
        struct Component { int data; };
        Database database;
        Scene scene(database);
        GameObject source = scene.CreateObject();
        auto sourceComponent = source.AddComponent<Component>(123);
        GameObject dest = scene.CreateObject();
        auto destComponent = dest.AddReferenceComponent<Component>(source);
        scene.Update(0.0f);
        return sourceComponent == destComponent;
    });
    
    RunTest("AddReferenceComponent, source removed", [] () -> bool {
        struct Component { int data; };
        Database database;
        Scene scene(database);
        GameObject source = scene.CreateObject();
        source.AddComponent<Component>(123);
        GameObject dest = scene.CreateObject();
        dest.AddReferenceComponent<Component>(source);
        source.Remove();
        scene.Update(0.0f);
        source = scene.CreateObject();
        auto sourceComponent = source.AddComponent<Component>(123);
        return dest.GetComponent<Component>()!=nullptr && dest.GetComponent<Component>()!=sourceComponent && sourceComponent;
    });
    
    RunTest("AddSystem", [] () -> bool {
        struct Position {};
        struct PositionSystem : public System<Position> { void Update(float dt) {}};
        Database database;
        Scene scene(database);
        auto* system = &scene.CreateSystem<PositionSystem>();
        return system != nullptr;
    });
    
    RunTest("System::ObjectAdded", [] () -> bool {
        using List = std::vector<GameObject>;
        List objectsAdded;
        struct Position {int x;};
        struct PositionSystem : public System<Position> {
            List* list;
            void Update(float dt) {}
            void ObjectAdded(GameObject object) {
                list->push_back(object);
            }
        };
        Database database;
        Scene scene(database);
        scene.CreateSystem<PositionSystem>().list = &objectsAdded;
        auto object = scene.CreateObject();
        object.AddComponent<Position>(123);
        scene.Update(0.0f);
        return objectsAdded.size() == 1 && objectsAdded[0]==object;
    });
    
    RunTest("System::ObjectRemoved", [] () -> bool {
        using List = std::vector<GameObject>;
        List objectsRemoved;
        struct Position {int x;};
        struct PositionSystem : public System<Position> {
            List* list;
            void Update(float dt) {}
            void ObjectRemoved(GameObject object) {
                list->push_back(object);
            }
        };
        Database database;
        Scene scene(database);
        scene.CreateSystem<PositionSystem>().list = &objectsRemoved;
        auto object = scene.CreateObject();
        object.AddComponent<Position>(123);
        object.RemoveComponent<Position>();
        scene.Update(0.0f);
        return objectsRemoved.size() == 1 && objectsRemoved[0]==object;
    });
    
    RunTest("Remove object from System on Scene destructor", [] () -> bool {
        struct Position { int pos; };
        
        struct MoveSystem : public System<Position> {
            int* addedPtr;
            int* removedPtr;
            
            void ObjectAdded(GameObject object) {
                (*addedPtr)++;
            }
            
            void ObjectRemoved(GameObject object) {
                (*removedPtr)++;
            }
            
            void Update(float dt) { }
        };
        
        int addedCounter = 0;
        int removedCounter = 0;
        
        Database database;
        GameObject gameObject;
        {
            Scene scene(database);
            auto& system = scene.CreateSystem<MoveSystem>();
            system.addedPtr = &addedCounter;
            system.removedPtr = &removedCounter;
            gameObject = scene.CreateObject();
            gameObject.AddComponent<Position>(456);
            scene.Update(0.0f);
        }
        return addedCounter == 1 && removedCounter == 1;
    });
    
     RunTest("Add and remove component from System in same update", [] () -> bool {
         struct Transform { int pos; };
         struct Renderable { int image; };
         struct RenderSystem : System<Transform, Renderable> {
             int numAdded = 0;
             int numRemoved = 0;
         
             void ObjectAdded(GameObject go) override {
                 numAdded++;
             }
             
             void ObjectRemoved(GameObject go) override {
                 numRemoved++;
             }
         };
         
         Database database;
         Scene scene(database);
         
         auto& renderSystem = scene.CreateSystem<RenderSystem>();
         
         GameObject go = scene.CreateObject();
         
         go.AddComponent<Transform>(12);
         go.AddComponent<Renderable>(12);
         
         go.RemoveComponent<Transform>();
         go.RemoveComponent<Renderable>();
         
         scene.Update(0.0f);
         return renderSystem.numAdded == 1 &&
             renderSystem.numRemoved == 1;
     });
    
    RunTest("Scene::Objects iteration", [] () -> bool {
        struct Component1 { int data;};
        struct Component2 { int data2;};
        
        Database database;
        Scene scene(database);
        
        std::vector<GameObject> objects;
        for(int i=0;i<10; i++) {
            auto go = scene.CreateObject();
            go.AddComponent<Component1>();
            objects.push_back(go);
        }
        
        bool match = true;
        for(auto go : scene.Objects()) {
            auto it = std::find(objects.begin(), objects.end(), go);
            if (it == objects.end()) {
                match = false;
            }
        }
        return match;
     });
    
    
     RunTest("System::ObjectRemoved, two systems, 3 components", [] () -> bool {
        using List = std::vector<GameObject>;
        List objectsRemoved;
        struct Transform {int x;};
        struct Velocity {int x;};
        struct Rotatable {int x;};
        
        struct VelocitySystem : public System<Transform, Velocity> {
            int removedCounter = 0;
            int addedCounter = 0;
            
            void ObjectAdded(GameObject object) {
                addedCounter++;
            }
            
            void ObjectRemoved(GameObject object) {
                removedCounter++;
            }
        };
        
        struct RotateSystem : public System<Transform, Rotatable> {
            int removedCounter = 0;
            int addedCounter = 0;
            
            void ObjectAdded(GameObject object) {
                addedCounter++;
            }
            
            void ObjectRemoved(GameObject object) {
                removedCounter++;
            }
        };
        Database database;
        Scene scene(database);
        auto& velocitySystem = scene.CreateSystem<VelocitySystem>();
        auto& rotationSystem = scene.CreateSystem<RotateSystem>();
        
        auto object = scene.CreateObject();
        object.AddComponent<Transform>();
        object.AddComponent<Velocity>();
        object.AddComponent<Rotatable>();
        
        scene.Update(0.0f);
        
        bool bothAdded = velocitySystem.addedCounter == 1 && rotationSystem.addedCounter == 1;
        
        object.RemoveComponent<Rotatable>();
        
        scene.Update(0.0f);
        
        bool onlyOneRemoved = velocitySystem.removedCounter == 0 && rotationSystem.removedCounter == 1;
        
        return bothAdded && onlyOneRemoved;
    });
}
