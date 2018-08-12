#include "RayCaster.h"

namespace Core {

    void RayCaster::addObject(WeakPointer<Object3D> sceneObject, WeakPointer<Mesh> mesh) {
        this->sceneObjects.push_back(sceneObject);
        this->meshes.push_back(mesh);
    }

}