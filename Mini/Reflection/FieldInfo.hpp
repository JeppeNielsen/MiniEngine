//
//  FieldInfo.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 29/11/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <iostream>
#include "FieldSerializer.hpp"

namespace Mini {
    class FieldInfo {
    public:
        
        FieldInfo() { }
    
        template <class T>
        FieldInfo(const std::string& name, T& data) : field(std::make_shared<Field<T>>(name, data)) {}

        void Serialize(minijson::object_writer& writer) const {
            field->Serialize(writer);
        }
        
        void Deserialize(minijson::istream_context& context, minijson::value jsonValue) const {
            field->Deserialize(context, jsonValue);
        }
        
        const std::string& Name() {
            return field->name;
        }
        
        template<typename T>
        T* GetField() {
            return (T*)field->Get();
        }
        
    private:
        struct IField {
            virtual ~IField() = default;
            virtual void Serialize(minijson::object_writer& writer) const = 0;
            virtual void Deserialize(minijson::istream_context& context, minijson::value jsonValue) const = 0;
            virtual void* Get() const = 0;
            std::string name;
        };
        template <class T>
        struct Field : IField {
            Field(const std::string& name, T& field) : field(&field) { this->name = name; }
            void Serialize(minijson::object_writer& writer) const override {
                FieldSerializer<T>::Serialize(writer, name, *field);
            }
            void Deserialize(minijson::istream_context& context, minijson::value jsonValue) const override {
                FieldSerializer<T>::Deserialize(context, jsonValue, *field);
            }
            void* Get() const override {
                return field;
            }
            T* field;
        };
        
        std::shared_ptr<const IField> field;
    };
}
