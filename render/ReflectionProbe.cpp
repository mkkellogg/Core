#include "ReflectionProbe.h"

namespace Core {

    ReflectionProbe::ReflectionProbe(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        needsUpdate = false;
    }

    void ReflectionProbe::init() {

    }

    void ReflectionProbe::setNeedsUpdate(Bool needsUpdate) {
        this->needsUpdate = needsUpdate;
    }

    Bool ReflectionProbe::getNeedsUpdate() {
        return this->needsUpdate;
    }

    void ReflectionProbe::update() {

    }
}
