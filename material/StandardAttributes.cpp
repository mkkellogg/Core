#include "StandardAttributes.h"

namespace Core {

    const std::string StandardAttributes::attributeNames[] =
    {
        "POSITION",
        "COLOR",
        "UV0",
        "UV1",
        "NORMAL",
        "TANGENT",
        "FACE_NORMAL",
    };

    std::unordered_map<std::string, StandardAttribute> StandardAttributes::nameToAttribute
    {
        {attributeNames[(UInt16)StandardAttribute::Position],StandardAttribute::Position},
        {attributeNames[(UInt16)StandardAttribute::Color],StandardAttribute::Color},
        {attributeNames[(UInt16)StandardAttribute::UV0],StandardAttribute::UV0},
        {attributeNames[(UInt16)StandardAttribute::UV1],StandardAttribute::UV1},
        {attributeNames[(UInt16)StandardAttribute::Normal],StandardAttribute::Normal},
        {attributeNames[(UInt16)StandardAttribute::Tangent],StandardAttribute::Tangent},
        {attributeNames[(UInt16)StandardAttribute::FaceNormal],StandardAttribute::FaceNormal}
        
    };

    StandardAttributeSet StandardAttributes::createAttributeSet() {
        return (StandardAttributeSet)IntMaskUtil::createMask();
    }

    void StandardAttributes::addAttribute(StandardAttributeSet * set, StandardAttribute attr) {
        IntMaskUtil::setBit((IntMask *)set, (IntMask)attr);
    }

    void StandardAttributes::removeAttribute(StandardAttributeSet * set, StandardAttribute attr) {
        IntMaskUtil::clearBit((IntMask *)set, (IntMask)attr);
    }

    Bool StandardAttributes::hasAttribute(StandardAttributeSet set, StandardAttribute attr) {
        return IntMaskUtil::isBitSet((IntMask)set, (IntMask)attr);
    }
}