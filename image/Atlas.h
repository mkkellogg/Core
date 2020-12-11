#pragma once

#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "Texture2D.h"

namespace Core {

    class Atlas {
    public:

        class TileArrayDescriptor {
        public:
            TileArrayDescriptor(UInt32 length, Real x, Real y, Real width, Real height) {
                this->length = length;
                this->x = x;
                this->y = y;
                this->width = width;
                this->height = height;
            }

            UInt32 length;
            Real x;
            Real y;
            Real width;
            Real height;
        };     

        Atlas();
        Atlas(WeakPointer<Texture2D> texture);
        virtual ~Atlas();

        void addTileArray(UInt32 length, Real x, Real y, Real width, Real height);

    private:
        PersistentWeakPointer<Texture2D> AtlasTexture;
        std::vector<TileArrayDescriptor> tileArrays;
    };
}
