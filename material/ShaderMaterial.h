#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;

    class ShaderMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
    
    protected:
        ShaderMaterial(const std::string& vertexShaderName, const std::string& fragmentShaderName, WeakPointer<Graphics> graphics);
        std::string vertexShaderName;
        std::string fragmentShaderName;
    };
}
