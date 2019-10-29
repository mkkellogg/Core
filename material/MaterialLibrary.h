#pragma once

#include <unordered_map>

#include "../util/PersistentWeakPointer.h"
#include "ShaderMaterialCharacteristic.h"
#include "../base/BitMask.h"

namespace Core {

    // forward declarations
    class Material;

    class MaterialLibrary {
    public:
        MaterialLibrary();

        void addEntry(LongMask shaderMaterialChacteristics, WeakPointer<Material> material);
        Bool hasMaterial(LongMask shaderMaterialChacteristics);
        WeakPointer<Material> getMaterial(LongMask shaderMaterialChacteristics);
    
    private:
        class Entry {
        public:
            PersistentWeakPointer<Material> material;
            LongMask shaderMaterialChacteristics;
        };

        std::unordered_map<LongMask, Entry> entries;
    };

}