#include "Atlas.h"

namespace Core {

    Atlas::Atlas() {

    }

    Atlas::Atlas(WeakPointer<Texture2D> texture) {
        this->texture = texture;
    }

    Atlas::~Atlas() {

    }

    WeakPointer<Texture2D> Atlas::getTexture() {
        return this->texture;
    }

    UInt32 Atlas::getTileArrayCount() const {
        return this->tileArrays.size();
    }

    void Atlas::addTileArray(UInt32 length, Real x, Real y, Real width, Real height) {
        this->tileArrays.push_back(TileArrayDescriptor(length, x, y, width, height));
    }

    Atlas::TileArrayDescriptor& Atlas::getTileArray(UInt32 index) {
        if (index >= this->tileArrays.size()) {
            throw OutOfRangeException("Atlas::getTileArray -> 'index' is out of range.");
        }
        return this->tileArrays[index];
    }
}