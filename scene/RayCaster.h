#pragma once

#include <vector>

#include "../geometry/Ray.h"
#include "../geometry/Hit.h"
#include "../scene/Object3D.h"
#include "../util/PersistentWeakPointer.h"
#include "../render/RenderableContainer.h"

namespace Core {

    class RayCaster {
    public:
        void addObject(WeakPointer<Object3D> sceneObject, WeakPointer<Mesh> mesh);
        Bool castRay(const Ray& ray, std::vector<Hit>& hits);
        Bool castRay(const Ray& ray, WeakPointer<Mesh> mesh, const Matrix4x4& transform, std::vector<Hit>& hits);


    private:
        std::vector<PersistentWeakPointer<Object3D>> objects;
        std::vector<PersistentWeakPointer<Mesh>> meshes;
    };
}