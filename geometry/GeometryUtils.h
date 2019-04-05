#pragma once

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../color/Color.h"
#include "../render/RenderableContainer.h"

namespace Core {

    // forward declarations
    class Mesh;
    class Material;

    class GeometryUtils {
    public:
       static  WeakPointer<Mesh> createGrid(Real width, Real height, UInt32 hSubDivisions, UInt32 vSubDivisions,
                                            Real hTexToWorld, Real vTexToWorld);
     
        static WeakPointer<Mesh> buildBoxMesh(Real length, Real width, Real depth, Color color);

        static WeakPointer<Mesh> buildArrowMesh(Real baseLength, Real baseRadius,
                                                Real coneLength, Real coneRadius,
                                                UInt32 subdivisions, Color color);

        static WeakPointer<Mesh> buildSphereMesh(Real radius, UInt32 subdivisions, Color color);

        static WeakPointer<Mesh> buildTorusMesh(Real radius, Real tubeRadius, UInt32 subdivisions, UInt32 tubeSubdivisions, Color color);

        static WeakPointer<RenderableContainer<Mesh>> buildMeshContainer(WeakPointer<Mesh> mesh,
                                                                    WeakPointer<Material> material,
                                                                    const std::string& name);
    private:
        

        static void generateTorusSection(Real radius, Real tubeRadius, Real angle, Real tubeAngleStart,
                                         Real tubeAngleEnd, Point3r& start, Point3r& end);
    };
}
