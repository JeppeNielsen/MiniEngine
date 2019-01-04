//
//  Rect.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Rect.hpp"
#include "StringHelper.hpp"
#include <vector>

using namespace Mini;

Rect Rect::Deserialize(const std::string &data, const char delim) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, ',', v);
    return Rect(v.size()>0 ? v[0] : 0,
                v.size()>1 ? v[1] : 0,
                v.size()>2 ? v[2] : 0,
                v.size()>3 ? v[3] : 0);
}

Rect Rect::Lerp(const Rect &a, const Rect &b, float t) {
    return Rect(a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.width + (b.width - a.width) * t,
                a.height + (b.height - a.height) * t);
}
