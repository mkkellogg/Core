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

    UInt32 Atlas::getFrameSetCount() const {
        return this->frameSets.size();
    }

    void Atlas::addFrameSet(UInt32 length, Real x, Real y, Real width, Real height) {
        this->frameSets.push_back(FrameSetDescriptor(length, x, y, width, height));
    }

    Atlas::FrameSetDescriptor& Atlas::getFrameSet(UInt32 index) {
        if (index >= this->frameSets.size()) {
            throw OutOfRangeException("Atlas::getFrameSet -> 'index' is out of range.");
        }
        return this->frameSets[index];
    }
}