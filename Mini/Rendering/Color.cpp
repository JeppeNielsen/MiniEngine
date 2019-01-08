#include "Color.hpp"
#include "StringHelper.hpp"

using namespace Mini;

Color Color::Deserialize(const std::string &data) {
    std::vector<int> v;
    StringHelper::splitToInts(data, ',', v);
    return Color(
                  (Component)(v.size()>0 ? v[0] : 0),
                  (Component)(v.size()>1 ? v[1] : 0),
                  (Component)(v.size()>2 ? v[2] : 0),
                  (Component)(v.size()>3 ? v[3] : 0)
                  );
}
