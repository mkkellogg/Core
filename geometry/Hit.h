#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Vector3.h"

namespace Core {

    // forward declarations
    class Mesh;

    class Hit {
    public:
        Vector3r Normal;
        Point3r Origin;
        Real Distance;
        PersistentWeakPointer<Mesh> Object;
    };

}