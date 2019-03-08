#include "Object3DComponent.h"

namespace Core {

    Object3DComponent::Object3DComponent(WeakPointer<Object3D> owner) : owner(owner), active(true) {
    }

    Object3DComponent::~Object3DComponent() {
    }

    void Object3DComponent::setActive(Bool active) {
        this->active = active;
    }

    Bool Object3DComponent::isActive() {
        return this->active;
    }
}