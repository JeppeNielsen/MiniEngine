//
//  JsonDeserializer.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 02/08/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "JsonDeserializer.hpp"
#include "Scene.hpp"
#include "minijson_writer.hpp"

using namespace Mini;

JsonDeserializer::JsonDeserializer() {
    
}

void JsonDeserializer::DeserializeObject(GameObject object, std::istream& stream) {
    
    minijson::istream_context context(stream);
    try {
        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string token = n;
            if (v.type() == minijson::Object && token == "GameObject") {
                Deserialize(object, context, [](GameObject o){
        
                });
            }
        });
    } catch (minijson::parse_error e) {
        std::cout << e.what() << " at position: " << e.offset() << std::endl;
    }
}

void JsonDeserializer::DeserializeComponents(GameObject object, std::istream& stream) {
    minijson::istream_context context(stream);
    try {
        DeserializeComponentsInternal(object, context);
    } catch (minijson::parse_error e) {
        std::cout << e.what() << " at position: " << e.offset() << std::endl;
    }
}

void JsonDeserializer::Deserialize(GameObject object, minijson::istream_context &context, const std::function<void(GameObject)>& objectCreated) {
    minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        std::string name = n;
        if (name == "id" && v.type() == minijson::Number) {
           // object->id = (int)v.as_long();
        } else if (name == "Components" && v.type() == minijson::Object) {
            DeserializeComponentsInternal(object, context);
        } else if (name == "Children" && v.type() == minijson::Array) {
            minijson::parse_array(context, [&] (minijson::value v) {
                if (v.type() == minijson::Object) {
                    GameObject child = object.Scene().CreateObject();
                    child.Hierarchy().Parent = object;
                    Deserialize(child, context, objectCreated);
                }
            });
        } else if (name == "guid" && v.type() == minijson::String) {
            //object->scene->guid = std::string(v.as_string());
        } else if (name == "counter" && v.type() == minijson::Number) {
            //object->scene->idCounter = (int)v.as_long();
        }
        
        if (objectCreated) {
            objectCreated(object);
        }
    });
}

void JsonDeserializer::DeserializeComponent(GameObject object, minijson::istream_context &context, const std::string &componentName){
    auto& database = object.Scene().GetDatabase();

    IContainer* container;
    int componentId;
    if (!database.TryFindComponentContainer(componentName, &container, componentId)) return;
    
    object.AddComponent(componentId);
    container->Deserialize(object.Id(), context);
}

void JsonDeserializer::DeserializeComponentsInternal(GameObject object, minijson::istream_context &context) {
    minijson::parse_object(context, [&] (const char* name, minijson::value v) {
        if (v.type() == minijson::Object) {
            DeserializeComponent(object, context, name);
        } else {
            minijson::ignore(context);
        }
    });
}
