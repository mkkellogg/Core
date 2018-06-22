#include "MaterialLibrary.h"

namespace Core {

    MaterialLibrary::MaterialLibrary() {

    }

    void MaterialLibrary::addEntry(LongMask shaderMaterialChacteristics, WeakPointer<Material> material) {
        if (this->entries.find(shaderMaterialChacteristics) == this->entries.end()) {
            Entry entry;
            entry.shaderMaterialChacteristics = shaderMaterialChacteristics;
            entry.material = material;
            this->entries[shaderMaterialChacteristics] = entry;
        }
    }

    Bool MaterialLibrary::hasMaterial(LongMask shaderMaterialChacteristics) {
        return this->entries.find(shaderMaterialChacteristics) != this->entries.end();
    }

    WeakPointer<Material> MaterialLibrary::getMaterial(LongMask shaderMaterialChacteristics) {
        if (this->hasMaterial(shaderMaterialChacteristics)) {
            return this->entries[shaderMaterialChacteristics].material;
        }
        return WeakPointer<Material>();
    }

}