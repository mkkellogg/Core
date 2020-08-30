#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../base/CoreObject.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class PointLight;
    class Object3D;
    class Mesh;

    class RenderUtils {
    public:

        static Bool isPointLightInRangeOfMesh(WeakPointer<PointLight> pointLight, Point3r& pointLightPos,
                                              WeakPointer<Mesh> mesh, WeakPointer<Object3D> meshOwner);

    };

}
