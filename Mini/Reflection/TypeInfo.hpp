//
//  TypeInfo.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 29/11/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "FieldInfo.hpp"
#include "ClassNameHelper.hpp"
#include "minijson_reader.hpp"
#include "minijson_writer.hpp"

namespace Mini {
    class TypeInfo {
    public:
        using FieldCollection = std::vector<FieldInfo>;
    
        TypeInfo(const std::string& name);
        void Serialize(minijson::object_writer& writer);
        void Deserialize(minijson::istream_context& context);
        const std::string& Name() const;
        bool TryFindField(const std::string& name, FieldInfo& info);
        
        template<typename T>
        void AddField(const std::string& name, T& field) {
            fields.emplace_back(FieldInfo(name, field));
        }
        
        const FieldCollection& Fields() const;
    private:
        std::string name;
        
        FieldCollection fields;
    };
    
#define TYPE_FIELDS_BEGIN \
public: \
Mini::TypeInfo GetType() { \
    const static auto& name = ClassNameHelper::GetName<std::remove_const_t<std::remove_pointer_t<decltype(this)>>>(); \
    Mini::TypeInfo info(name);

#define TYPE_FIELD(field) \
    info.AddField(#field, this->field);

#define TYPE_FIELDS_END \
        return info; \
     \
} \
private:
}
