#include <vector>

#include "../Engine.h"
#include "../geometry/IndexBuffer.h"
#include "../geometry/Mesh.h"
#include "GeometryUtils.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Core {

    WeakPointer<Mesh> GeometryUtils::createGrid(Real width, Real height, UInt32 hCells, UInt32 vCells, Real hWorldToTex, Real vWorldToTex) {
        Real halfWidth = width / 2.0f;
        Real halfHeight = height / 2.0f;

        Real cellWidth = width / (Real)hCells;
        Real cellHeight = height / (Real)vCells;

        Real left = -halfWidth;
        Real bottom = -halfHeight;

        Real textureCellWidth = hWorldToTex / cellWidth;
        Real textureCellHeight = vWorldToTex / cellHeight;

        std::vector<Real> positions;
        std::vector<Real> normals;
        std::vector<Real> uvs;
        std::vector<UInt32> indices;

        for (UInt32 cx = 0; cx <= hCells; cx++) {
            for (UInt32 cy = 0; cy <= vCells; cy++) {
                Real xOffset = (Real)cx * cellWidth;
                Real yOffset = (Real)cy * cellHeight;
                Real bx = xOffset + left;
                Real by = yOffset + bottom;

                Real uvX = xOffset / hWorldToTex;
                Real uvY = yOffset / vWorldToTex;

                positions.push_back(bx);
                positions.push_back(by);
                positions.push_back(0);
                positions.push_back(1);

                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(1);
                normals.push_back(0);

                uvs.push_back(uvX);
                uvs.push_back(uvY);
            }
        }

        for (UInt32 cx = 0; cx < hCells; cx++) {
            for (UInt32 cy = 0; cy < vCells; cy++) {
                Real bx = (Real)cx * cellWidth + left;
                Real by = (Real)cy * cellHeight + bottom;

                UInt32 i0 = cx * (vCells + 1) + cy;
                UInt32 i1 = i0 + 1;
                UInt32 i2 = (cx + 1) * (vCells + 1) + cy;
                UInt32 i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i3);

                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i2);

            }
        }

        UInt32 vertexCount = (hCells + 1) * (vCells + 1);

        WeakPointer<Engine> engine = Engine::instance();

        WeakPointer<Mesh> gridPlane = engine->createMesh(vertexCount, hCells * vCells * 6);

        gridPlane->enableAttribute(Core::StandardAttribute::Position);
        Core::Bool positionInited = gridPlane->initVertexPositions();
        gridPlane->getVertexPositions()->store(positions.data());
        
        gridPlane->enableAttribute(Core::StandardAttribute::Normal);
        Core::Bool normalsInited = gridPlane->initVertexNormals();
        gridPlane->getVertexNormals()->store(normals.data());

        gridPlane->enableAttribute(Core::StandardAttribute::UV0);
        Core::Bool uvsInited = gridPlane->initVertexUVs();
        gridPlane->getVertexUVs()->store(uvs.data());

        gridPlane->getIndexBuffer()->setIndices(indices.data());

        return gridPlane;
    }
}