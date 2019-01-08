//
//  Colorable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/27/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once

#include "Property.hpp"
#include "Color.hpp"
#include "TypeInfo.hpp"

namespace Mini {
    class Colorable {
    public:
        Colorable();
        Property<Color> Color;
        
        TYPE_FIELDS_BEGIN;
        TYPE_FIELD(Color);
        TYPE_FIELDS_END;
    };
}
