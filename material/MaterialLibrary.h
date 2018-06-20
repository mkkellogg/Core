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
            LongMask attributes;
        };

        MaterialLibrary();

        void addEntry(LongMask attributes, WeakPointer<Material> material);
    
    private:
        std::unordered_map<LongMask, Entry> entries;
    };

}