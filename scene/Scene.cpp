#include "Scene.h"
#include "Object3D.h"
#include "../common/assert.h"
#include "../common/complextypes.h"

namespace Core {

    Scene::Scene(WeakPointer<Object3D> root) : root(root) {
    }

    WeakPointer<Object3D> Scene::getRoot() {
        return this->root;
    }

    void Scene::visitScene(WeakPointer<Object3D> object, VisitorCallback callback) {
        if (object == WeakPointer<Object3D>::nullPtr) return;
        callback(object);

        for (SceneObjectIterator<Object3D> itr = object->beginIterateChildren(); itr != object->endIterateChildren(); ++itr) {
            WeakPointer<Object3D> obj = *itr;
            visitScene(obj, callback);
        }
    }
}
