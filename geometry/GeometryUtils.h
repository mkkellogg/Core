#pragma once

#include "../common/types.h"
#include "../util/WeakPointer.h"

namespace Core {

    // forward declarations
    class Mesh;

    class GeometryUtils {
    public:
       static  WeakPointer<Mesh> createGrid(Real width, Real height, UInt32 hSubDivisions, UInt32 vSubDivisions, Real hTexToWorld, Real vTexToWorld);
     
    private:
        
    };
}
