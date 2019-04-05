#include <vector>

#include "GeometryUtils.h"
#include "../Engine.h"
#include "../geometry/IndexBuffer.h"
#include "../geometry/Mesh.h"
#include "../math/Math.h"
#include "../material/StandardAttributes.h"
#include "../render/MeshRenderer.h"
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

        gridPlane->enableAttribute(Core::StandardAttribute::AlbedoUV);
        Core::Bool uvsInited = gridPlane->initVertexAlbedoUVs();
        gridPlane->getVertexAlbedoUVs()->store(uvs.data());

        gridPlane->getIndexBuffer()->setIndices(indices.data());

        return gridPlane;
    }
    
    WeakPointer<Mesh> GeometryUtils::buildBoxMesh(Real length, Real height, Real depth, Color color) {

        Real halfLength = length / 2.0f;
        Real halfHeight = height / 2.0f;
        Real halfDepth = depth / 2.0f;

        Real vertexPositions[] = {
            // back
            -halfLength, -halfHeight, -halfDepth, 1.0, halfLength, halfHeight, -halfDepth, 1.0, -halfLength, halfHeight, -halfDepth, 1.0,
            -halfLength, -halfHeight, -halfDepth, 1.0, halfLength, -halfHeight, -halfDepth, 1.0, halfLength, halfHeight, -halfDepth, 1.0,
            // left
            -halfLength, -halfHeight, -halfDepth, 1.0, -halfLength, halfHeight, -halfDepth, 1.0, -halfLength, -halfHeight, halfDepth, 1.0,
            -halfLength, halfHeight, -halfDepth, 1.0, -halfLength, halfHeight, halfDepth, 1.0, -halfLength, -halfHeight, halfDepth, 1.0,
            // right
            halfLength, -halfHeight, -halfDepth, 1.0, halfLength, -halfHeight, halfDepth, 1.0, halfLength, halfHeight, -halfDepth, 1.0,
            halfLength, halfHeight, -halfDepth, 1.0, halfLength, -halfHeight, halfDepth, 1.0, halfLength, halfHeight, halfDepth, 1.0,
            // top
            -halfLength, halfHeight, -halfDepth, 1.0, halfLength, halfHeight, -halfDepth, 1.0, -halfLength, halfHeight, halfDepth, 1.0,
            halfLength, halfHeight, -halfDepth, 1.0, halfLength, halfHeight, halfDepth, 1.0, -halfLength, halfHeight, halfDepth, 1.0,
            // bottom
            -halfLength, -halfHeight, -halfDepth, 1.0, -halfLength, -halfHeight, halfDepth, 1.0, halfLength, -halfHeight, halfDepth, 1.0,
            -halfLength, -halfHeight, -halfDepth, 1.0, halfLength, -halfHeight, halfDepth, 1.0, halfLength, -halfHeight, -halfDepth, 1.0,
            // front
            halfLength, halfHeight, halfDepth, 1.0, -halfLength, -halfHeight, halfDepth, 1.0, -halfLength, halfHeight, halfDepth, 1.0,
            halfLength, halfHeight, halfDepth, 1.0, halfLength, -halfHeight, halfDepth, 1.0, -halfLength, -halfHeight, halfDepth, 1.0
        };

        Real vertexNormals[] = {
            // back
            0.0, 0.0, -1.0, 0.0,  0.0, 0.0, -1.0, 0.0,  0.0, 0.0, -1.0, 0.0,
            0.0, 0.0, -1.0, 0.0,  0.0, 0.0, -1.0, 0.0,  0.0, 0.0, -1.0, 0.0,
            // left
            -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0,
            -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0,
            // right
            1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            // top
            0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
            // bottom
            0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0,
            0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0,
            // front
            0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 1.0, 0.0,
        };

        Real vertexColors[] = {
            // back
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            // left
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            // right
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            // top
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            // bottom
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            // front
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a
        };

        WeakPointer<Engine> engine = Engine::instance();

        WeakPointer<Mesh> boxMesh(engine->createMesh(36, 0));
        boxMesh->init();
        boxMesh->enableAttribute(StandardAttribute::Position);
        Bool positionInited = boxMesh->initVertexPositions();
        ASSERT(positionInited, "Unable to initializebox mesh vertex positions.");
        boxMesh->getVertexPositions()->store(vertexPositions);

        boxMesh->enableAttribute(StandardAttribute::Color);
        Bool colorInited = boxMesh->initVertexColors();
        ASSERT(colorInited, "Unable to initialize box mesh colors.");
        boxMesh->getVertexColors()->store(vertexColors);

        boxMesh->enableAttribute(StandardAttribute::Normal);
        Bool normalInited = boxMesh->initVertexNormals();
        ASSERT(normalInited, "Unable to initialize box vertex normals.");
        boxMesh->getVertexNormals()->store(vertexNormals);

        boxMesh->enableAttribute(StandardAttribute::FaceNormal);
        Bool faceNormalInited = boxMesh->initVertexFaceNormals();

        boxMesh->calculateBoundingBox();

        return boxMesh;
    }

    WeakPointer<Mesh> GeometryUtils::buildArrowMesh(Real baseLength, Real baseRadius,
                                                                Real coneLength, Real coneRadius,
                                                                UInt32 subdivisions, Color color) {

        UInt32 facesPerSide = 6;
        UInt32 faceCount = subdivisions * facesPerSide;
        UInt32 verticesPerSide = facesPerSide * 3;
        UInt32 componentsPerSide = verticesPerSide * 4;
        UInt32 vertexCount = faceCount * 3;
        UInt32 componentCount = vertexCount * 4;
        Real vertices[componentCount];
        Real colors[componentCount];

        Real totalLength = baseLength + coneLength;
        Real halfLength = totalLength / 2.0f;

        Real lastBaseX = 0.0f;
        Real lastBaseY = 0.0f;
        Real lastConeX = 0.0f;
        Real lastConeY = 0.0f;
        for (UInt32 s = 0; s <= subdivisions; s++) {
            Real t = static_cast<Real>(s) / static_cast<Real>(subdivisions);
            float angle = t * Math::TwoPI;
            Real x = Math::cos(angle);
            Real y = Math::sin(angle);
            if (s == subdivisions) {
                x = 1.0f;
                y = 0.0;
            }

            Real baseX = x * baseRadius;
            Real baseY = y * baseRadius;
            Real coneX = x * coneRadius;
            Real coneY = y * coneRadius;

            if (s >= 1) {
                UInt32 index = (s - 1) * componentsPerSide;

                // bottom of base
                vertices[index] = baseX;
                vertices[index + 1] = -halfLength;
                vertices[index + 2] = baseY;
                vertices[index + 3] = 1.0;
                vertices[index + 4] = lastBaseX;
                vertices[index + 5] = -halfLength;
                vertices[index + 6] = lastBaseY;
                vertices[index + 7] = 1.0;
                vertices[index + 8] = 0.0;
                vertices[index + 9] = -halfLength;
                vertices[index + 10] = 0.0;
                vertices[index + 11] = 1.0;

                // side of base
                vertices[index + 12] = lastBaseX;
                vertices[index + 13] = -halfLength;
                vertices[index + 14] = lastBaseY;
                vertices[index + 15] = 1.0;
                vertices[index + 16] = baseX;
                vertices[index + 17] = -halfLength;
                vertices[index + 18] = baseY;
                vertices[index + 19] = 1.0;
                vertices[index + 20] = baseX;
                vertices[index + 21] = baseLength - halfLength;
                vertices[index + 22] = baseY;
                vertices[index + 23] = 1.0;

                vertices[index + 24] = baseX;
                vertices[index + 25] = baseLength - halfLength;
                vertices[index + 26] = baseY;
                vertices[index + 27] = 1.0;
                vertices[index + 28] = lastBaseX;
                vertices[index + 29] = baseLength - halfLength;
                vertices[index + 30] = lastBaseY;
                vertices[index + 31] = 1.0;
                vertices[index + 32] = lastBaseX;
                vertices[index + 33] = -halfLength;
                vertices[index + 34] = lastBaseY;
                vertices[index + 35] = 1.0;

                // bottom of cone
                vertices[index + 36] = lastBaseX;
                vertices[index + 37] = baseLength - halfLength;
                vertices[index + 38] = lastBaseY;
                vertices[index + 39] = 1.0;
                vertices[index + 40] = baseX;
                vertices[index + 41] = baseLength - halfLength;
                vertices[index + 42] = baseY;
                vertices[index + 43] = 1.0;
                vertices[index + 44] = coneX;
                vertices[index + 45] = baseLength - halfLength;
                vertices[index + 46] = coneY;
                vertices[index + 47] = 1.0;

                vertices[index + 48] = coneX;
                vertices[index + 49] = baseLength - halfLength;
                vertices[index + 50] = coneY;
                vertices[index + 51] = 1.0;
                vertices[index + 52] = lastConeX;
                vertices[index + 53] = baseLength - halfLength;
                vertices[index + 54] = lastConeY;
                vertices[index + 55] = 1.0;
                vertices[index + 56] = lastBaseX;
                vertices[index + 57] = baseLength - halfLength;
                vertices[index + 58] = lastBaseY;
                vertices[index + 59] = 1.0;

                // cone face
                vertices[index + 60] = lastConeX;
                vertices[index + 61] = baseLength - halfLength;
                vertices[index + 62] = lastConeY;
                vertices[index + 63] = 1.0;
                vertices[index + 64] = coneX;
                vertices[index + 65] = baseLength - halfLength;
                vertices[index + 66] = coneY;
                vertices[index + 67] = 1.0;
                vertices[index + 68] = 0.0;
                vertices[index + 69] = baseLength + coneLength - halfLength;
                vertices[index + 70] = 0.0;
                vertices[index + 71] = 1.0;
            }

            for (UInt32 v = 0; v < vertexCount; v++) {
                UInt32 index = v * 4;
                colors[index] = color.r;
                colors[index + 1] = color.g;
                colors[index + 2] = color.b;
                colors[index + 3] = color.a;
            }

            lastBaseX = baseX;
            lastBaseY = baseY;
            lastConeX = coneX;
            lastConeY = coneY;
        }

        WeakPointer<Engine> engine = Engine::instance();

        WeakPointer<Mesh> arrowMesh(engine->createMesh(vertexCount, 0));
        arrowMesh->init();
        arrowMesh->enableAttribute(StandardAttribute::Position);
        Bool positionInited = arrowMesh->initVertexPositions();
        ASSERT(positionInited, "Unable to initialize arrow mesh vertex positions.");
        arrowMesh->getVertexPositions()->store(vertices);

        arrowMesh->enableAttribute(StandardAttribute::Color);
        Bool colorInited = arrowMesh->initVertexColors();
        ASSERT(colorInited, "Unable to initialize arrow mesh colors.");
        arrowMesh->getVertexColors()->store(colors);

        arrowMesh->enableAttribute(StandardAttribute::Normal);
        Bool normalInited = arrowMesh->initVertexNormals();
        ASSERT(normalInited, "Unable to initialize arrow vertex normals.");

        arrowMesh->enableAttribute(StandardAttribute::FaceNormal);
        Bool faceNormalInited = arrowMesh->initVertexFaceNormals();

        arrowMesh->calculateBoundingBox();
        arrowMesh->calculateNormals(75.0f);

        return arrowMesh;
    }

    WeakPointer<Mesh> GeometryUtils::buildSphereMesh(Real radius, UInt32 subdivisions, Color color) {
        UInt32 hSubdivisions = subdivisions;
        UInt32 vSubdivisions = subdivisions / 4;

        std::vector<Real> positions;
        std::vector<Real> normals;
        std::vector<Real> colors;

        Real halfPI = Math::PI / 2.0f;
        Real deltaTheta = Math::TwoPI / (Real)hSubdivisions;
        Real deltaPhi = halfPI / (Real)vSubdivisions;

        for (UInt32 i = 0; i < hSubdivisions; i++) {
            for (UInt32 j = 0; j < vSubdivisions; j++) {
                Real theta = (Real)i * deltaTheta;
                Real nextTheta = theta + deltaTheta;
                Real phi = (Real)j * deltaPhi;
                Real nextPhi = phi + deltaPhi;

                Real bottomY = Math::sin(phi) * radius;
                Real topY = Math::sin(nextPhi) * radius;

                Real projectedRadius = radius * Math::sin(halfPI - phi);
                Real nextProjectedRadius = radius * Math::sin(halfPI - nextPhi);

                Real bottomStartX = Math::cos(theta) * projectedRadius;
                Real bottomEndX = Math::cos(nextTheta) * projectedRadius;
                Real bottomStartZ = Math::sin(theta) * projectedRadius;
                Real bottomEndZ = Math::sin(nextTheta) * projectedRadius;

                if (j == vSubdivisions - 1) {
                    positions.push_back(bottomStartX);  
                    positions.push_back(bottomY);  
                    positions.push_back(bottomStartZ);
                    positions.push_back(1.0f); 

                    positions.push_back(bottomEndX);  
                    positions.push_back(bottomY);  
                    positions.push_back(bottomEndZ);  
                    positions.push_back(1.0f); 

                    positions.push_back(0);  
                    positions.push_back(radius);  
                    positions.push_back(0);
                    positions.push_back(1.0f);


                    positions.push_back(bottomStartX);  
                    positions.push_back(-bottomY);  
                    positions.push_back(bottomStartZ); 
                    positions.push_back(1.0f);

                    positions.push_back(0);  
                    positions.push_back(-radius);  
                    positions.push_back(0);   
                    positions.push_back(1.0f);

                    positions.push_back(bottomEndX);  
                    positions.push_back(-bottomY);  
                    positions.push_back(bottomEndZ);   
                    positions.push_back(1.0f);
                }
                else {
                    Real topStartX = Math::cos(theta) * nextProjectedRadius;
                    Real topEndX = Math::cos(nextTheta) * nextProjectedRadius;
                    Real topStartZ = Math::sin(theta) * nextProjectedRadius;
                    Real topEndZ = Math::sin(nextTheta) * nextProjectedRadius;

                    positions.push_back(bottomStartX);  
                    positions.push_back(bottomY);  
                    positions.push_back(bottomStartZ); 
                    positions.push_back(1.0f);

                    positions.push_back(bottomEndX);  
                    positions.push_back(bottomY);  
                    positions.push_back(bottomEndZ); 
                    positions.push_back(1.0f); 

                    positions.push_back(topStartX);  
                    positions.push_back(topY);  
                    positions.push_back(topStartZ); 
                    positions.push_back(1.0f); 

                    positions.push_back(topStartX);  
                    positions.push_back(topY);  
                    positions.push_back(topStartZ);
                    positions.push_back(1.0f);  

                    positions.push_back(bottomEndX);  
                    positions.push_back(bottomY);  
                    positions.push_back(bottomEndZ);
                    positions.push_back(1.0f);  

                    positions.push_back(topEndX);  
                    positions.push_back(topY);  
                    positions.push_back(topEndZ); 
                    positions.push_back(1.0f);




                    positions.push_back(bottomStartX);  
                    positions.push_back(-bottomY);  
                    positions.push_back(bottomStartZ); 
                    positions.push_back(1.0f); 

                    positions.push_back(topStartX);  
                    positions.push_back(-topY);  
                    positions.push_back(topStartZ); 
                    positions.push_back(1.0f);

                    positions.push_back(bottomEndX);  
                    positions.push_back(-bottomY);  
                    positions.push_back(bottomEndZ); 
                    positions.push_back(1.0f);
 
                    positions.push_back(topStartX);  
                    positions.push_back(-topY);  
                    positions.push_back(topStartZ);  
                    positions.push_back(1.0f);

                    positions.push_back(topEndX);  
                    positions.push_back(-topY);  
                    positions.push_back(topEndZ); 
                    positions.push_back(1.0f); 

                    positions.push_back(bottomEndX);  
                    positions.push_back(-bottomY);  
                    positions.push_back(bottomEndZ);
                    positions.push_back(1.0f);  
                }       
            }
        }

        for (UInt32 i = 0; i < positions.size(); i+=4) {
            Real x = positions[i];
            Real y = positions[i + 1];
            Real z = positions[i + 2];
            Vector3r normal(x, y, z);
            normal.normalize();
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
            normals.push_back(0.0f);
            colors.push_back(color.r);
            colors.push_back(color.g);
            colors.push_back(color.b);
            colors.push_back(color.a);
        }

        WeakPointer<Engine> engine = Engine::instance();

        UInt32 vertexCount = positions.size() / 4;

        WeakPointer<Mesh> sphereMesh(engine->createMesh(vertexCount, 0));
        sphereMesh->init();
        sphereMesh->enableAttribute(StandardAttribute::Position);
        Bool positionInited = sphereMesh->initVertexPositions();
        ASSERT(positionInited, "Unable to initialize sphere mesh vertex positions.");
        sphereMesh->getVertexPositions()->store(positions.data());

        sphereMesh->enableAttribute(StandardAttribute::Color);
        Bool colorInited = sphereMesh->initVertexColors();
        ASSERT(colorInited, "Unable to initialize sphere mesh colors.");
        sphereMesh->getVertexColors()->store(colors.data());

        sphereMesh->enableAttribute(StandardAttribute::Normal);
        Bool normalInited = sphereMesh->initVertexNormals();
        ASSERT(normalInited, "Unable to initialize sphere vertex normals.");

        sphereMesh->enableAttribute(StandardAttribute::FaceNormal);
        Bool faceNormalInited = sphereMesh->initVertexFaceNormals();

        sphereMesh->calculateBoundingBox();
        sphereMesh->calculateNormals(85.0f);

        return sphereMesh;
    }

    WeakPointer<Mesh> GeometryUtils::buildTorusMesh(Real radius, Real tubeRadius, UInt32 subdivisions, UInt32 tubeSubdivisions, Color color) {

        std::vector<Real> positions;
        std::vector<Real> normals;
        std::vector<Real> colors;

        Real deltaTheta = Math::TwoPI / (Real)subdivisions;
        Real deltaPhi = Math::TwoPI  / (Real)tubeSubdivisions;

        for (UInt32 i = 0; i < subdivisions; i++) {
            UInt32 nextI = i < subdivisions - 1 ? i + 1 : 0;
            for (UInt32  j = 0; j < tubeSubdivisions; j++) {
                UInt32 nextJ = j < tubeSubdivisions - 1 ? j + 1 : 0;
                
                Real theta = (Real)i * deltaTheta;
                Real nextTheta = (Real)nextI * deltaTheta;
                Real phi = (Real)j * deltaPhi;
                Real nextPhi = (Real)nextJ * deltaPhi;

                Point3r bottom;
                Point3r top;
                Point3r nextbottom;
                Point3r nextTop;

                generateTorusSection(radius, tubeRadius, theta, phi, nextPhi, bottom, top);
                generateTorusSection(radius, tubeRadius, nextTheta, phi, nextPhi, nextbottom, nextTop);

                positions.push_back(bottom.x);  
                positions.push_back(bottom.y);  
                positions.push_back(bottom.z); 
                positions.push_back(1.0f);

                positions.push_back(nextbottom.x);  
                positions.push_back(nextbottom.y);  
                positions.push_back(nextbottom.z); 
                positions.push_back(1.0f); 

                positions.push_back(top.x);  
                positions.push_back(top.y);  
                positions.push_back(top.z); 
                positions.push_back(1.0f); 

                positions.push_back(nextbottom.x);  
                positions.push_back(nextbottom.y);  
                positions.push_back(nextbottom.z); 
                positions.push_back(1.0f); 

                positions.push_back(nextTop.x);  
                positions.push_back(nextTop.y);  
                positions.push_back(nextTop.z); 
                positions.push_back(1.0f); 

                positions.push_back(top.x);  
                positions.push_back(top.y);  
                positions.push_back(top.z); 
                positions.push_back(1.0f); 

            }
        }

        for (UInt32 i = 0; i < positions.size(); i+=4) {
            colors.push_back(color.r);
            colors.push_back(color.g);
            colors.push_back(color.b);
            colors.push_back(color.a);
        }

        WeakPointer<Engine> engine = Engine::instance();

        UInt32 vertexCount = positions.size() / 4;

        WeakPointer<Mesh> torusMesh(engine->createMesh(vertexCount, 0));
        torusMesh->init();
        torusMesh->enableAttribute(StandardAttribute::Position);
        Bool positionInited = torusMesh->initVertexPositions();
        ASSERT(positionInited, "Unable to initialize torus mesh vertex positions.");
        torusMesh->getVertexPositions()->store(positions.data());

        torusMesh->enableAttribute(StandardAttribute::Color);
        Bool colorInited = torusMesh->initVertexColors();
        ASSERT(colorInited, "Unable to initialize torus mesh colors.");
        torusMesh->getVertexColors()->store(colors.data());

        torusMesh->enableAttribute(StandardAttribute::Normal);
        Bool normalInited = torusMesh->initVertexNormals();
        ASSERT(normalInited, "Unable to initialize torus vertex normals.");

        torusMesh->enableAttribute(StandardAttribute::FaceNormal);
        Bool faceNormalInited = torusMesh->initVertexFaceNormals();

        torusMesh->calculateBoundingBox();
        torusMesh->calculateNormals(85.0f);

        return torusMesh;

    }

    void GeometryUtils::generateTorusSection(Real radius, Real tubeRadius, Real angle, Real tubeAngleStart,
                                             Real tubeAngleEnd, Point3r& start, Point3r& end) {
        Real baseStartX = Math::cos(tubeAngleStart) * tubeRadius;
        Real finalStartY = Math::sin(tubeAngleStart) * tubeRadius;
        Real baseEndX = Math::cos(tubeAngleEnd) * tubeRadius;
        Real finalEndY = Math::sin(tubeAngleEnd) * tubeRadius;

        Real finalStartX = (radius + baseStartX) * Math::cos(angle);
        Real finalEndX = (radius + baseEndX) * Math::cos(angle);

        Real finalStartZ = (radius + baseStartX) * Math::sin(angle);
        Real finalEndZ = (radius + baseEndX) * Math::sin(angle);

        start.set(finalStartX, finalStartY, finalStartZ);
        end.set(finalEndX, finalEndY, finalEndZ);
    }

    WeakPointer<RenderableContainer<Mesh>> GeometryUtils::buildMeshContainer(WeakPointer<Mesh> mesh,
                                                                             WeakPointer<Material> material,
                                                                             const std::string& name) {
        WeakPointer<Engine> engine = Engine::instance();
        WeakPointer<RenderableContainer<Mesh>> obj(engine->createObject3D<RenderableContainer<Mesh>>());
        obj->setName(name);
        WeakPointer<MeshRenderer> renderer = engine->createRenderer<MeshRenderer>(material, obj);
        obj->addRenderable(mesh);
        return obj;
    }
}