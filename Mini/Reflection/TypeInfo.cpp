//
//  TypeInfo.cpp
//  ECS
//
//  Created by Jeppe Nielsen on 29/11/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "TypeInfo.hpp"

using namespace Mini;

TypeInfo::TypeInfo(const std::string& name) : name(name) { }

void TypeInfo::Serialize(minijson::object_writer& writer) {
    for(auto& field : fields) {
        field.Serialize(writer);
    }
}

void TypeInfo::Deserialize(minijson::istream_context& context) {
    minijson::parse_object(context, [&] (const char* name, minijson::value jsonValue) {
        FieldInfo field;
        if (TryFindField(name, field)) {
            field.Deserialize(context, jsonValue);
        }
    });
}

const std::string& TypeInfo::Name() {
    return name;
}

bool TypeInfo::TryFindField(const std::string& name, FieldInfo& info) {
    for(auto& field : fields) {
        if (field.Name() == name) {
            info = field;
            return true;
        }
    }
    return false;
}
