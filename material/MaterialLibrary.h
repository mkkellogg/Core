#pragma once

#include <unordered_map>

#include "../util/PersistentWeakPointer.h"
#include "../Graphics.h"
#include "ShaderMaterialCharacteristic.h"
#include "Material.h"
#include "../base/BitMask.h"

namespace Core {

    class MaterialLibrary {
    public:
        class Entry {
        public:
            PersistentWeakPointer<Material> material;
            LongMask shaderMaterialChacteristics;
        };

        MaterialLibrary();

        void addEntry(LongMask shaderMaterialChacteristics, WeakPointer<Material> material);
        Bool hasMaterial(LongMask shaderMaterialChacteristics);
        WeakPointer<Material> getMaterial(LongMask shaderMaterialChacteristics);
    
    private:
        std::unordered_map<LongMask, Entry> entries;
    };

}