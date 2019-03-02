#include "StandardAttributes.h"

namespace Core {

    std::shared_ptr<StandardAttributes> StandardAttributes::instance = nullptr;

    StandardAttributes::StandardAttributes() {
        attributeNames =
        {
            "POSITION",
            "COLOR",
            "ALBEDOUV",
            "NORMALUV",
            "NORMAL",
            "AVERAGED_NORMAL",
            "TANGENT",
            "FACE_NORMAL",
        };

        nameToAttribute =
        {
            {attributeNames[(UInt16)StandardAttribute::Position],StandardAttribute::Position},
            {attributeNames[(UInt16)StandardAttribute::Color],StandardAttribute::Color},
            {attributeNames[(UInt16)StandardAttribute::AlbedoUV],StandardAttribute::AlbedoUV},
            {attributeNames[(UInt16)StandardAttribute::NormalUV],StandardAttribute::NormalUV},
            {attributeNames[(UInt16)StandardAttribute::Normal],StandardAttribute::Normal},
            {attributeNames[(UInt16)StandardAttribute::AveragedNormal],StandardAttribute::AveragedNormal},
            {attributeNames[(UInt16)StandardAttribute::Tangent],StandardAttribute::Tangent},
            {attributeNames[(UInt16)StandardAttribute::FaceNormal],StandardAttribute::FaceNormal}
            
        };
    }

    const std::string& StandardAttributes::getAttributeName(StandardAttribute attribute) {
        checkAndInitInstance();
        return instance->_getAttributeName(attribute);
    }

    const std::string& StandardAttributes::_getAttributeName(StandardAttribute attribute) {
        return attributeNames[(UInt16)attribute];
    }

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

    void StandardAttributes::checkAndInitInstance() {
        if (!instance) {
           instance = std::shared_ptr<StandardAttributes>(new StandardAttributes());
           instance->init();
        }
    }

    void StandardAttributes::init() {

    }
}