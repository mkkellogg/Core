#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "../base/CoreObject.h"

namespace Core {

    // forward declaration
    class Object3D;

    class Object3DComponent: public CoreObject{
    public:
        Object3DComponent(WeakPointer<Object3D> owner);
        virtual ~Object3DComponent();

        WeakPointer<Object3D> getOwner() {
            return this->owner;
        }
        void setActive(Bool active);
        Bool isActive();

    protected:
        PersistentWeakPointer<Object3D> owner;
        Bool active;
    };
}
