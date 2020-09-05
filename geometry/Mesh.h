#pragma once

#include <new>
#include <unordered_map>
#include <vector>

#include "../Graphics.h"
#include "../render/BaseRenderable.h"
#include "../util/PersistentWeakPointer.h"
#include "../color/Color.h"
#include "../common/assert.h"
#include "../common/types.h"
#include "../material/StandardAttributes.h"
#include "AttributeArray.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Box3.h"

namespace Core {

    // forward declarations
    class Engine;
    class Object3D;
    class IndexBuffer;

    class Mesh : public BaseRenderable {
        friend class Engine;

    public:
        virtual ~Mesh();
        virtual void init();

        UInt32 getVertexCount() const;
        UInt32 getIndexCount() const;

        WeakPointer<AttributeArray<Point3rs>> getVertexPositions();
        WeakPointer<AttributeArray<Vector3rs>> getVertexNormals();
        WeakPointer<AttributeArray<Vector3rs>> getVertexAveragedNormals();
        WeakPointer<AttributeArray<Vector3rs>> getVertexFaceNormals();
        WeakPointer<AttributeArray<Vector3rs>> getVertexTangents();
        WeakPointer<AttributeArray<ColorS>> getVertexColors();
        WeakPointer<AttributeArray<Vector2rs>> getVertexAlbedoUVs();
        WeakPointer<AttributeArray<Vector2rs>> getVertexNormalUVs();
        WeakPointer<IndexBuffer> getIndexBuffer();

        Bool initVertexPositions();
        Bool initVertexNormals();
        Bool initVertexFaceNormals();
        Bool initVertexTangents();
        Bool initVertexColors();
        Bool initVertexAlbedoUVs();
        Bool initVertexNormalUVs();

        void enableAttribute(StandardAttribute attribute);
        void disableAttribute(StandardAttribute attribute);
        Bool isAttributeEnabled(StandardAttribute attribute);
        Bool isIndexed();

        void calculateBoundingBox();
        const Box3& getBoundingBox() const;

        void calculateBoundingSphere();
        const Vector4r& getBoundingSphere() const;

        void setNormalsSmoothingThreshold(Real threshold);
        void setCalculateNormals(Bool calculateNormals);
        void setCalculateTangents(Bool calculateTangents);
        void setCalculateBounds(Bool calculateBoundingBox);
        void calculateNormals(Real smoothingThreshold);
        void calculateTangents(Real smoothingThreshhold);

        void setName(const std::string& name);
        const std::string& getName() const;

        void update();
        void reverseVertexAttributeWindingOrder();

    protected:
        Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, UInt32 indexCount);
        void initAttributes();
        Bool initIndices();
        void calculateFaceNormal(UInt32 faceIndex, Vector3r& result);
        void calculateTangent(UInt32 vertexIndex, UInt32 rightIndex, UInt32 leftIndex, Vector3r& result);
        void destroyVertexCrossMap();
        Bool buildVertexCrossMap();

        template <typename T>
        Bool initVertexAttributes(std::shared_ptr<AttributeArray<T>>* attributes, UInt32 vertexCount) {          
            try {
                *attributes = std::make_shared<AttributeArray<T>>(vertexCount);
            }
            catch(...) {
                throw AllocationException("MeshGL::initVertexAttributes() -> Unable to allocate array.");
            }

            WeakPointer<AttributeArrayGPUStorage> gpuStorage =
                Engine::instance()->createGPUStorage((*attributes)->getSize(), T::ComponentCount, AttributeType::Float, false);
            (*attributes)->setGPUStorage(gpuStorage);
            return true;
        }

        std::string name;
        PersistentWeakPointer<Graphics> graphics;
        Bool initialized;
        StandardAttributeSet enabledAttributes;
        UInt32 vertexCount;
        Bool indexed;
        UInt32 indexCount;
        Box3 boundingBox;
        Vector4r boundingSphere;

        std::shared_ptr<AttributeArray<Point3rs>> vertexPositions;
        std::shared_ptr<AttributeArray<Vector3rs>> vertexNormals;
        std::shared_ptr<AttributeArray<Vector3rs>> vertexAveragedNormals;
        std::shared_ptr<AttributeArray<Vector3rs>> vertexFaceNormals;
        std::shared_ptr<AttributeArray<Vector3rs>> vertexTangents;
        std::shared_ptr<AttributeArray<ColorS>> vertexColors;
        std::shared_ptr<AttributeArray<Vector2rs>> vertexAlbedoUVs;
        std::shared_ptr<AttributeArray<Vector2rs>> vertexNormalUVs;

        PersistentWeakPointer<IndexBuffer> indexBuffer;

        // maps vertices to other equal vertices
        std::vector<UInt32>** vertexCrossMap;
        Bool shouldCalculateNormals;
        Bool shouldCalculateTangents;
        Bool shouldCalculateBounds;
        Real normalsSmoothingThreshold;

    };
}
