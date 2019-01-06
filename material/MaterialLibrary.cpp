#include <bitset>

#include "MaterialLibrary.h"
#include "Material.h"

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
        for (auto entry : this->entries) {
            if (LongMaskUtil::covers(entry.first, shaderMaterialChacteristics)) return true;
        }
        return false;
    }

    WeakPointer<Material> MaterialLibrary::getMaterial(LongMask shaderMaterialChacteristics) {
        for (auto entry : this->entries) {
            if (LongMaskUtil::covers(entry.first, shaderMaterialChacteristics)) return entry.second.material;
        }
        return WeakPointer<Material>();
    }

}