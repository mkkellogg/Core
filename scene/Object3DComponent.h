#pragma once

#include <memory>

#include "../util/WeakPointer.h"

namespace Core {

    // forward declaration
    class Object3D;

    class Object3DComponent {
    public:
        Object3DComponent(WeakPointer<Object3D> owner);
        virtual ~Object3DComponent();

        WeakPointer<Object3D> getOwner() {
            return this->owner;
        }

    protected:
        WeakPointer<Object3D> owner;
    };
}
