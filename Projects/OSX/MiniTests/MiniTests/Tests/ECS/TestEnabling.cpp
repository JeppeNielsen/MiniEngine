//
//  TestEnabling.cpp
//  MiniTests
//
//  Created by Jeppe Nielsen on 18/06/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "TestEnabling.hpp"
#include "ECS.hpp"

using namespace Mini::ECS;
using namespace Tests::ECS;

void TestEnabling::Run() {
    
    RunTest("Hierarchy.Enabled is true by default", [] () {
        Database database;
        Scene scene(database);
        auto object = scene.CreateObject();
        return object.Hierarchy().Enabled();
    });
    
    RunTest("Hierarchy.WorldEnabled follow Enabled state", [] () {
        Database database;
        Scene scene(database);
        auto object = scene.CreateObject();
        object.Hierarchy().Enabled = false;
        bool wasDisabled = !object.Hierarchy().WorldEnabled();
        object.Hierarchy().Enabled = true;
        bool isEnabled = object.Hierarchy().WorldEnabled();
        return wasDisabled && isEnabled;
    });
    
    RunTest("Hierarchy.WorldEnabled set to false, from disabled parent", [] () {
        Database database;
        Scene scene(database);
        auto parent = scene.CreateObject();
        parent.Hierarchy().Enabled = false;
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        return child.Hierarchy().Enabled() && !child.Hierarchy().WorldEnabled();
    });

    RunTest("Remove from system, when Enabled = false", [] () {
        struct Renderable {
            int meshId;
        };
        
        int numAdded = 0;
        int numRemoved = 0;
        
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
        
        
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto object = scene.CreateObject();
        object.AddComponent<Renderable>(10);
        
        scene.Update(0);
        bool wasAdded = numAdded == 1;
        
        object.Hierarchy().Enabled = false;
        
        scene.Update(0);
        
        bool wasRemoved = numRemoved == 1;
        
        return wasAdded && wasRemoved;
    });
    
    RunTest("Remove from system, when reparented to disabled parent", [] () {
        struct Renderable {
            int meshId;
        };
        
        int numAdded = 0;
        int numRemoved = 0;
        
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
        
        
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto parent = scene.CreateObject();
        parent.Hierarchy().Enabled = false;
        
        auto child = scene.CreateObject();
        child.AddComponent<Renderable>(10);
        
        scene.Update(0);
        bool wasAdded = numAdded == 1;
        
        child.Hierarchy().Parent = parent;
        
        scene.Update(0);
        
        bool wasRemoved = numRemoved == 1;
        
        return wasAdded && wasRemoved;
    });
    
    RunTest("Dont add to system when disabled", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
       
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
        };
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
    
        auto child = scene.CreateObject();
        child.AddComponent<Renderable>(10);
        child.Hierarchy().Enabled = false;
    
        scene.Update(0);
        
        return numAdded == 0;
    });
    
    
    RunTest("Multiple enable/disable results in one ObjectAdded", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
        int numRemoved = 0;
    
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
    
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
    
        auto child = scene.CreateObject();
        child.AddComponent<Renderable>(10);
        child.Hierarchy().Enabled = false;
        child.Hierarchy().Enabled = true;
        child.Hierarchy().Enabled = false;
        child.Hierarchy().Enabled = true;
        scene.Update(0);
        return numAdded == 1 && numRemoved == 0;
    });
    
    
    RunTest("Dont add to system if added to disabled parent, AddComponent is post enable", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
        int numRemoved = 0;
    
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
    
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto parent = scene.CreateObject();
        parent.Hierarchy().Enabled = false;
    
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        
        child.AddComponent<Renderable>(10);
        scene.Update(0);
        return numAdded == 0 && numRemoved == 0;
    });
    
    RunTest("Dont add to system if added to disabled parent, AddComponent is pre enable", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
        int numRemoved = 0;
    
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
    
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto parent = scene.CreateObject();
        parent.Hierarchy().Enabled = false;
    
        auto child = scene.CreateObject();
        child.AddComponent<Renderable>(10);
        child.Hierarchy().Parent = parent;
        
        scene.Update(0);
        return numAdded == 0 && numRemoved == 0;
    });
    
    RunTest("Add to system if gameobject's parent moved out of it's disabled parent", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
        int numRemoved = 0;
    
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
    
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto grandParent = scene.CreateObject();
        grandParent.Hierarchy().Enabled = false;
        
        auto parent = scene.CreateObject();
        parent.Hierarchy().Parent = grandParent;
    
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        child.AddComponent<Renderable>(10);
        
        scene.Update(0);
        
        bool wasNotAdded = numAdded == 0;
        
        parent.Hierarchy().Parent = nullptr;
        
        scene.Update(0);
        
        return wasNotAdded && numAdded == 1 && numRemoved == 0;
    });
    
    
    RunTest("Remove from system if gameobject's parent moved into a disabled parent", [] () {
        struct Renderable {
            int meshId;
        };
    
        int numAdded = 0;
        int numRemoved = 0;
    
        struct RenderSystem : System<Renderable> {
            int* numAdded;
            int* numRemoved;
            
            void ObjectAdded(GameObject object) override {
                (*numAdded)++;
            }
            
            void ObjectRemoved(GameObject object) override {
                (*numRemoved)++;
            }
        };
    
    
        Database database;
        Scene scene(database);
        auto& r = scene.CreateSystem<RenderSystem>();
        r.numAdded = &numAdded;
        r.numRemoved = &numRemoved;
        
        auto grandParent = scene.CreateObject();
        grandParent.Hierarchy().Enabled = false;
        
        auto parent = scene.CreateObject();
        parent.Hierarchy().Parent = nullptr;
    
        auto child = scene.CreateObject();
        child.Hierarchy().Parent = parent;
        child.AddComponent<Renderable>(10);
        
        scene.Update(0);
        
        bool wasAdded = numAdded == 1;
        
        parent.Hierarchy().Parent = grandParent;
        
        scene.Update(0);
        
        return wasAdded && numRemoved == 1;
    });

}
