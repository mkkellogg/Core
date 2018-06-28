#include <vector>

#include "GeometryUtils.h"
#include "../Engine.h"
#include "../geometry/Mesh.h"
#include "../geometry/IndexBuffer.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Core {

    WeakPointer<Mesh> GeometryUtils::createGrid(Real width, Real height, UInt32 hCells, UInt32 vCells, Real hWorldToTex, Real vWorldToTex) {
        Real halfWidth = width / 2.0f;
        Real halfHeight = height / 2.0f;

        Real cellWidth = width / (Real) hCells;
        Real cellHeight = height / (Real) vCells;

        Real left = -halfWidth;
        Real bottom = -halfHeight;

        Real textureCellWidth = hWorldToTex / cellWidth;
        Real textureCellHeight = vWorldToTex / cellHeight;

        std::vector<Real> positions;
        std::vector<Real> normals;
        std::vector<Real> uvs;
        std::vector<UInt32> indices;

        /*for (UInt32 cx = 0; cx < hCells; cx++) {
            for (UInt32 cy = 0; cy < vCells; cy++) {
                Real bx = (Real)cx * cellWidth + left;
                Real by = (Real)cy * cellHeight + bottom;

                Real uvX = bx / hWorldToTex;
                Real uvY = by / vWorldToTex;

                if (cx == 0 || cy == 0) {
                    // lower left of cell
                    positions.push_back(bx);
                    positions.push_back(by);
                    positions.push_back(0);

                    normals.push_back(0);
                    normals.push_back(0);
                    normals.push_back(1);

                    uvs.push_back(uvX);
                    uvs.push_back(uvY);
                }

                if (cx == 0) {
                    // upper left of cell
                    positions.push_back(bx);
                    positions.push_back(by + cellHeight);
                    positions.push_back(0);

                    normals.push_back(0);
                    normals.push_back(0);
                    normals.push_back(1);

                    uvs.push_back(uvX);
                    uvs.push_back(uvY + textureCellHeight);
                }
                else if (cy == 0) {
                    // lower right of cell
                    positions.push_back(bx + cellWidth);
                    positions.push_back(by);
                    positions.push_back(0);
                    
                    normals.push_back(0);
                    normals.push_back(0);
                    normals.push_back(1);

                    uvs.push_back(uvX + textureCellWidth);
                    uvs.push_back(uvY);
                }

                // upper right of cell
                positions.push_back(bx + cellWidth);
                positions.push_back(by + cellHeight);
                positions.push_back(0);

                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(1);
                
                uvs.push_back(uvX + textureCellWidth);
                uvs.push_back(uvY + textureCellHeight);
            } 
        }*/

         for (UInt32 cx = 0; cx <= hCells; cx++) {
            for (UInt32 cy = 0; cy <= vCells; cy++) {
                Real bx = (Real)cx * cellWidth + left;
                Real by = (Real)cy * cellHeight + bottom;

                positions.push_back(bx);
                positions.push_back(by);
                positions.push_back(0);

                normals.push_back(0);
                normals.push_back(0);
                normals.push_back(1);

                Real uvX = bx / hWorldToTex;
                Real uvY = by / vWorldToTex;

                uvs.push_back(uvX);
                uvs.push_back(uvY);
            }
         }

         for (UInt32 cx = 0; cx < hCells; cx++) {
            for (UInt32 cy = 0; cy < vCells; cy++) {
                Real bx = (Real)cx * cellWidth + left;
                Real by = (Real)cy * cellHeight + bottom;

                UInt32 i0 = cx * vCells + cy;
                UInt32 i1 = i0 + 1;
                UInt32 i2 = (cx + 1) * vCells + cy;
                UInt32 i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i1);

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
            }
         }

        UInt32 vertexCount = (hCells + 1) * (vCells + 1);    

        WeakPointer<Engine> engine = Engine::instance(); 

        WeakPointer<Mesh> gridPlane = engine->createMesh(vertexCount, true);
        gridPlane->init();

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