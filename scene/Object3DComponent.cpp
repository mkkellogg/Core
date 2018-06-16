#include "Object3DComponent.h"

namespace Core {

    Object3DComponent::Object3DComponent(WeakPointer<Object3D> owner) : owner(owner) {
    }

    Object3DComponent::~Object3DComponent() {
    }
}