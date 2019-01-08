//
//  FieldSerializer.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 20/12/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "minijson_writer.hpp"
#include "minijson_reader.hpp"
#include "GameObject.hpp"
#include "Property.hpp"
#include <sstream>
#include <map>

namespace Mini {
    template<typename T, typename I = void>
    struct FieldSerializer {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const T& field) {
            //writer.write(name.c_str(), field);
            std::stringstream s;
            s<<field;
            writer.write(name.c_str(), s.str());
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, T& field) {
            //minijson::ignore(context);
            
            if (value.type() != minijson::String) return;
            field = T::Deserialize(value.as_string());
        }
    };
  
    template<>
    struct FieldSerializer<int> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const int& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, int& field) {
            if (value.type() != minijson::Number) return;
            field = (int)value.as_long();
        }
    };
    
    template<>
    struct FieldSerializer<float> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const float& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, float& field) {
            if (value.type() != minijson::Number) return;
            field = (float)value.as_double();
        }
    };
    
    template<>
    struct FieldSerializer<short> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const short& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, short& field) {
            if (value.type() != minijson::Number) return;
            field = (short)value.as_long();
        }
    };
    
    template<>
    struct FieldSerializer<std::string> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const std::string& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, std::string& field) {
            if (value.type() != minijson::String) return;
            field = value.as_string();
        }
    };
    
    template<>
    struct FieldSerializer<bool> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const bool& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, bool& field) {
            if (value.type() != minijson::Boolean) return;
            field = value.as_bool();
        }
    };
    
    template<>
    struct FieldSerializer<uint64_t> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const uint64_t& field) {
            writer.write(name.c_str(), field);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, uint64_t& field) {
            if (value.type() != minijson::Number) return;
            field = ((uint64_t)value.as_long());
        }
    };
    
    template<typename T>
    struct FieldSerializer<std::vector<T>> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const std::vector<T>& vector) {
            auto array = writer.nested_array(name.c_str());
            for(const auto& item : vector) {
                FieldSerializer<T>::Serialize(array, name, item);
            }
            array.close();
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, std::vector<T>& vector) {
            minijson::parse_array(context, [&](minijson::value jsonValue) {
                vector.push_back(T{});
                auto& item = vector.back();
                FieldSerializer<T>::Deserialize(context, jsonValue, item);
            });
        }
    };
    
    template <typename T>
    struct HasGetTypeMethod {
        struct dummy { };

        template <typename C, typename P>
        static auto test(P * p) -> decltype(std::declval<C>().GetType(), std::true_type());

        template <typename, typename>
        static std::false_type test(...);

        typedef decltype(test<T, dummy>(nullptr)) type;
        static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
    };
    
    template<typename T>
    struct FieldSerializer<T, typename std::enable_if< HasGetTypeMethod<T>::value >::type> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const T& getTypeObject) {
            auto nestedObject = writer.nested_object(name.c_str());
       
            auto type = ((T&)getTypeObject).GetType();
            type.Serialize(nestedObject);
            
            nestedObject.close();
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, T& getTypeObject) {
            auto type = getTypeObject.GetType();
            type.Deserialize(context);
        }
    };
    
    template<>
    struct FieldSerializer<ECS::GameObject> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const ECS::GameObject& field) {
            writer.write(name.c_str(), "GameObject");
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, ECS::GameObject& field) {
            if (value.type() != minijson::String) return;
        //    field = value.as_string();
        }
    };
    
    template<typename T>
    struct FieldSerializer<Property<T>> {
        static void Serialize(minijson::object_writer& writer, const std::string& name, const Property<T>& field) {
            FieldSerializer<T>::Serialize(writer, name, field());
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, Property<T>& field) {
           T data;
           FieldSerializer<T>::Deserialize(context, value, data);
           field.operator=(data);
        }
    };
    
    template<typename Key, typename Value>
    struct FieldSerializer<std::map<Key, Value>> {
        static void Serialize(minijson::object_writer& writer, const std::string& key, const std::map<Key, Value>& value) {
            minijson::object_writer array = writer.nested_array(key.c_str());
            for (auto it = value.begin(); it!=value.end(); ++it) {
                minijson::object_writer keyValueArray = array.nested_array("");
                const Key& keyPointer = (const Key&)(it->first);
                FieldSerializer<Key>::Serialize(keyValueArray, "", keyPointer);
                const Value& valuePointer = (const Value&)(it->second);
                FieldSerializer<Value>::Serialize(keyValueArray, "", valuePointer);
                keyValueArray.close();
            }
            array.close();
        }
    
        static void Deserialize(minijson::istream_context& context, minijson::value& value, std::map<Key, Value>& map) {
            if (value.type() != minijson::Array) return;
            minijson::parse_array(context, [&] (minijson::value v) {
                if (v.type() != minijson::Array) {
                    minijson::ignore(context);
                } else {
                    int counter = 0;
                    Key key;
                    minijson::parse_array(context, [&] (minijson::value v) {
                        if (counter == 0) {
                            FieldSerializer<Key>::Deserialize(context, v, key);
                        } else if (counter == 1) {
                            Value& mapValue = map[key];
                            FieldSerializer<Value>::Deserialize(context, v, mapValue);
                        }
                        counter++;
                    });
                }
            });
        }
    };
    
    template<typename T>
    struct FieldSerializer<T, typename std::enable_if< std::is_enum<T>::value >::type> {
        static void Serialize(minijson::object_writer& writer, const std::string& key, const T& value) {
            writer.write(key.c_str(), (int)value);
        }
        
        static void Deserialize(minijson::istream_context& context, minijson::value& value, T& field) {
            if (value.type()!=minijson::Number) return;
            T enumValue = (T)value.as_long();
            field = enumValue;
        }
    };
    
}
