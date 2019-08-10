//
//  JsonSerializer.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 02/08/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include <ostream>
#include <functional>
#include "minijson_writer.hpp"

namespace Mini {
    using namespace ECS;
    class JsonSerializer {
    public:
        using CustomHeaderFunction = std::function<void(minijson::object_writer&)>;
    
        JsonSerializer();
        JsonSerializer(const CustomHeaderFunction& customHeaderFunction);
        void SerializeObject(GameObject go, std::ostream& stream, const std::function<bool(int)>& componentFilter = nullptr);
        void SerializeComponents(GameObject go, std::ostream& stream, const std::function<bool(int)>& componentFilter = nullptr);
    private:
        void Serialize(GameObject go, minijson::object_writer& writer, const std::function<bool(int)>& componentFilter);
        void SerializeComponentsInternal(GameObject go, minijson::object_writer& writer, const std::function<bool(int)>& componentFilter);
        CustomHeaderFunction customHeaderFunction;
    };
}
