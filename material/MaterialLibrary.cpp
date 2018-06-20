#include "MaterialLibrary.h"

namespace Core {

    MaterialLibrary::MaterialLibrary() {

    }

    void MaterialLibrary::addEntry(LongMask attributes, WeakPointer<Material> material) {
        if (this->entries.find(attributes) != this->entries.end()) {
            Entry entry;
            entry.material = attributes;
            entry.material = material;
            this->entries[attributes] = entry;
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