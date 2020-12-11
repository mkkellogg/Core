
#include "Atlas.h"

namespace Core {

    Atlas::Atlas() {
    }

    Atlas::~Atlas() {

    }

    void Atlas::addTileArray(UInt32 length, Real x, Real y, Real width, Real height) {
        this->tileArrays.push_back(TileArrayDescriptor(length, x, y, width, height));
    }
}