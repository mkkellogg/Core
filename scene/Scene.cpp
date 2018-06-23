#include "Scene.h"
#include "../common/assert.h"

namespace Core {

    Scene::Scene(WeakPointer<Object3D> root) : root(root) {
    }

    WeakPointer<Object3D> Scene::getRoot() {
        return this->root;
    }
}
