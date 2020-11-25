#include "AmbientPhysicalMaterial.h"

namespace Core {

    AmbientPhysicalMaterial::AmbientPhysicalMaterial(): StandardPhysicalMaterial("AmbientPhysical") {
        this->roughness = 0.8f;
        this->metallic = 0.05f;
    }
}