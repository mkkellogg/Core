#pragma once

#include <unordered_map>
#include <string>

#include "../base/BitMask.h"

namespace Core {

    enum class StandardAttribute {
        Position = 0,
        Color = 1,
        UV0 = 2,
        UV1 = 3,
        Normal = 4,
        Tangent = 5,
        FaceNormal = 6,
        _Count = 7,  // Must always be last in the list ( before _None);
        _None = 8,
    };

    typedef IntMask StandardAttributeSet;

    class StandardAttributes {
        static const std::string attributeNames[];
        static std::unordered_map<std::string, StandardAttribute> nameToAttribute;

    public:
        static StandardAttributeSet createAttributeSet();
        static void addAttribute(StandardAttributeSet* set, StandardAttribute attr);
        static void removeAttribute(StandardAttributeSet* set, StandardAttribute attr);
        static Bool hasAttribute(StandardAttributeSet set, StandardAttribute attr);
    };
}
