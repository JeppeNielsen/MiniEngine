//
//  ClassNameHelper.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 27/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>

class ClassNameHelper {

    template<typename Class>
    static std::string GetNameInternal() {
#ifdef WIN32
        const std::string functionName = __FUNCTION__;
#else
        const std::string functionName = __PRETTY_FUNCTION__;
#endif
        const std::string token = "Class = ";
        size_t equal = functionName.find(token) + token.size();
        return functionName.substr(equal, functionName.size() - equal - 1);
    }
    
public:
    template<typename T>
    static std::string& GetName() {
        static std::string name = GetNameInternal<T>();
        return name;
    }
};
