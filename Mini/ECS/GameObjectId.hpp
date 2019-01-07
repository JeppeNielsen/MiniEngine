//
//  GameObjectId.hpp
//  ECS
//
//  Created by Jeppe Nielsen on 25/07/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <cstdint>

namespace Mini {
namespace ECS {

using GameObjectId = std::uint32_t;
static constexpr auto GameObjectIdIndexMask = 0xFFFFF;
static constexpr auto GameObjectIdNull = ~0;

}
}

