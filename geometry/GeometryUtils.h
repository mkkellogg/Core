#pragma once

#include "../common/types.h"
#include "../util/WeakPointer.h"

namespace Core {

    // forward declarations
    class Mesh;

    class GeometryUtils {
    public:
       static  WeakPointer<Mesh> createGrid(Real width, Real height, UInt32 hSubDivisions, UInt32 vSubDivisions, Real hTexToWorld, Real vTexToWorld);
     
        static WeakPointer<Mesh> buildBoxMesh(Real length, Real width, Real depth, Color color);

        static WeakPointer<Mesh> buildArrowMesh(Real baseLength, Real baseRadius,
                                                Real coneLength, Real coneRadius,
                                                UInt32 subdivisions, Color color);
        static WeakPointer<RenderableContainer<Mesh>> buildMeshContainer(WeakPointer<Mesh> mesh,
                                                                    WeakPointer<Material> material,
                                                                    const std::string& name);
    private:
        
    };
}
