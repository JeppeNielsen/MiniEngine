//
//  MetaHelper.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Mini {
namespace Meta {

template<std::size_t I = 0, typename Tuple, typename Func>
typename std::enable_if< I == std::tuple_size<Tuple>::value, void >::type
for_each(const Tuple& tuple, Func&& func) {
}

template<std::size_t I = 0, typename Tuple, typename Func>
typename std::enable_if< I != std::tuple_size<Tuple>::value, void >::type
for_each(const Tuple& tuple, Func&& func) {
    func(std::get<I>(tuple));
    for_each<I + 1>(tuple, func);
}

template<typename Cond, typename Param, typename Func>
typename std::enable_if< Cond::value, void >::type
static_if(Param param, Func const&& func) {
    func(param);
}

template<typename Cond, typename Param, typename Func>
typename std::enable_if< !Cond::value, void >::type
static_if(Param param, Func const&& func) {
}

}
}
