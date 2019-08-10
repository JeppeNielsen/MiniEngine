//
//  JsonSerializer.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 02/08/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "JsonSerializer.hpp"
#include "Scene.hpp"

using namespace Mini;

JsonSerializer::JsonSerializer() {
    
}

JsonSerializer::JsonSerializer(const CustomHeaderFunction& customHeaderFunction)
: customHeaderFunction(customHeaderFunction) {
    
}

void JsonSerializer::SerializeObject(GameObject go, std::ostream& stream, const std::function<bool(int)>& componentFilter) {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    
    if (customHeaderFunction) {
        //auto info = writer.nested_object("Info");
        customHeaderFunction(writer);
        //info.close();
    }
    
    
    Serialize(go, writer, componentFilter);
    writer.close();
}

void JsonSerializer::SerializeComponents(GameObject go, std::ostream &stream, const std::function<bool(int)>& componentFilter) {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    SerializeComponentsInternal(go, writer, componentFilter);
    writer.close();
}

void JsonSerializer::Serialize(GameObject go, minijson::object_writer& writer, const std::function<bool(int)>& componentFilter) {

    auto gameObject = writer.nested_object("GameObject");
    auto components = gameObject.nested_object("Components");
    SerializeComponentsInternal(go, components, componentFilter);
    components.close();
    
    if (!go.Hierarchy().Children().empty()) {
        auto children = gameObject.nested_array("Children");
        for(auto child : go.Hierarchy().Children()) {
            Serialize(child, children, componentFilter);
        }
        children.close();
    }
    gameObject.close();
}

void JsonSerializer::SerializeComponentsInternal(GameObject go, minijson::object_writer &writer, const std::function<bool (int)> &componentFilter) {
    go.IterateComponentsWithIndex([go, &writer, &componentFilter](auto componentType, auto id) {
        if (Database::IdHelper::GetId<Mini::Hierarchy>() == id) return;
        if (componentFilter && !componentFilter(id)) return;
        componentType->Serialize(go.Id(), writer);
    });
}
