#include <vector>

#include "GeometryUtils.h"
#include "../Engine.h"
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

        for (UInt32 cx = 0; cx < hCells; cx++) {
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
        }

        UInt32 vertexCount = (hCells + 1) * (vCells + 1);    

        WeakPointer<Engine> engine = Engine::instance(); 

        WeakPointer<Mesh> gridPlane = engine->createMesh(vertexCount, false);
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

        return gridPlane;
    }

}