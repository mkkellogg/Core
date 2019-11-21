#include "AmbientPhysicalMaterial.h"

namespace Core {

    AmbientPhysicalMaterial::AmbientPhysicalMaterial(WeakPointer<Graphics> graphics): StandardPhysicalMaterial("AmbientPhysical", graphics) {
        this->roughness = 0.8f;
        this->metallic = 0.05f;
    }

    Bool AmbientPhysicalMaterial::build() {
        return StandardPhysicalMaterial::build();
    }
}