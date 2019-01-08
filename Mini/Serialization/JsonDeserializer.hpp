//
//  JsonSerializer.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 02/08/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Scene.hpp"
#include <istream>
#include "minijson_reader.hpp"

namespace Mini {
    using namespace ECS;
    class JsonDeserializer {
    public:
        JsonDeserializer();
        void DeserializeObject(GameObject object, std::istream& stream);
        void DeserializeComponents(GameObject object, std::istream& stream);
    private:
        void Deserialize(GameObject object, minijson::istream_context &context, const std::function<void(GameObject)>& objectCreated);
        void DeserializeComponent(GameObject object, minijson::istream_context &context, const std::string& componentName);
        void DeserializeComponentsInternal(GameObject object, minijson::istream_context &context);
    };
}
