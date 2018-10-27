#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

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

    public:
        static const std::string& getAttributeName(StandardAttribute attribute);
        static StandardAttributeSet createAttributeSet();
        static void addAttribute(StandardAttributeSet* set, StandardAttribute attr);
        static void removeAttribute(StandardAttributeSet* set, StandardAttribute attr);
        static Bool hasAttribute(StandardAttributeSet set, StandardAttribute attr);

    private:
        StandardAttributes();
        void init();
        static std::shared_ptr<StandardAttributes> instance;
        static void checkAndInitInstance();

        const std::string& _getAttributeName(StandardAttribute attribute);
        
        std::vector<std::string> attributeNames;
        std::unordered_map<std::string, StandardAttribute> nameToAttribute;
    };
}
