#pragma once

#include <vector>

#include "../scene/Object3D.h"
#include "../util/WeakPointer.h"
#include "../render/RenderableContainer.h"

namespace Core {

    class RayCaster {
    public:
        void addObject(WeakPointer<Object3D> sceneObject, WeakPointer<Mesh> mesh);
        
    private:
        std::vector<WeakPointer<Object3D>> sceneObjects;
        std::vector<WeakPointer<Mesh>> meshes;
    };
}