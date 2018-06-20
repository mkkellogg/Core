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

}