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

        WeakPointer<Mesh> boxMesh(engine->createMesh(36, false));
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
                // Vector3r botA(baseX, 0.0, baseY);
                // Vector3r botB(lastBaseX, 0.0, lastBaseY);
                // Vector3r botC(0.0, 0.0, 0.0);
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
                //Vector3r sideA(lastBaseX, 0.0, lastBaseY);
                //Vector3r sideB(baseX, 0.0, baseY);
                //Vector3r sideC(baseX, baseLength, baseY);
                //Vector3r sideD(baseX, baseLength, baseY);
                //Vector3r sideE(lastBaseX, baseLength, lastBaseY);
                //Vector3r sideF(lastBaseX, 0.0, lastBaseY);
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
                //Vector3r botConeA(lastBaseX, baseLength, lastBaseY);
                //Vector3r botConeB(baseX, baseLength, baseY);
                //Vector3r botConeC(coneX, baseLength, coneY);
                //Vector3r botConeD(coneX, baseLength, coneY);
                //Vector3r botConeE(lastConeX, baseLength, lastConeY);
                //Vector3r botConeF(lastBaseX, baseLength, lastBaseY);

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
                //Vector3r coneA(lastConeX, baseLength, lastConeY);
                //Vector3r coneB(coneX, baseLength, coneY);
                //Vector3r coneC(0.0, baseLength + coneLength, 0.0);
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

        WeakPointer<Mesh> arrowMesh(engine->createMesh(vertexCount, false));
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