#pragma once

#include <new>
#include <unordered_map>
#include <vector>

#include "../Graphics.h"
#include "../render/Renderable.h"
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

    class Mesh : public Renderable<Mesh> {
        friend class Engine;

    public:
        virtual ~Mesh();
        virtual void init();

        UInt32 getVertexCount() const;
        UInt32 getIndexCount() const;

        AttributeArray<Point3rs>* getVertexPositions();
        AttributeArray<Vector3rs>* getVertexNormals();
        AttributeArray<Vector3rs>* getVertexFaceNormals();
        AttributeArray<ColorS>* getVertexColors();
        AttributeArray<Vector2rs>* getVertexUVs0();
        WeakPointer<IndexBuffer> getIndexBuffer();

        Bool initVertexPositions();
        Bool initVertexNormals();
        Bool initVertexFaceNormals();
        Bool initVertexColors();
        Bool initVertexUVs0();

        void enableAttribute(StandardAttribute attribute);
        void disableAttribute(StandardAttribute attribute);
        Bool isAttributeEnabled(StandardAttribute attribute);
        Bool isIndexed();

        void calculateBoundingBox();
        const Box3& getBoundingBox() const;

        void setNormalsSmoothingThreshold(Real threshold);
        void setCalculateNormals(Bool calculateNormals);
        void calculateNormals(Real smoothingThreshold);

        void update();

    protected:
        Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, UInt32 indexCount);
        void initAttributes();
        Bool initIndices();
        void calculateFaceNormal(UInt32 faceIndex, Vector3r& result);
        void destroyVertexCrossMap();
        Bool buildVertexCrossMap();

        template <typename T>
        void destroyAttributeArray(AttributeArray<T>** attributes) {
            if (*attributes) {
                delete *attributes;
                *attributes = nullptr;
            }
        }

        template <typename T>
        Bool initVertexAttributes(AttributeArray<T>** attributes, UInt32 vertexCount) {
            this->destroyAttributeArray(attributes);

            *attributes = new (std::nothrow) AttributeArray<T>(vertexCount);
            if (*attributes == nullptr) {
                throw AllocationException("MeshGL::initVertexAttributes() -> Unable to allocate array.");
            }

            std::shared_ptr<AttributeArrayGPUStorage> gpuStorage =
                this->graphics->createGPUStorage((*attributes)->getSize(), T::ComponentCount, AttributeType::Float, false);
            (*attributes)->setGPUStorage(gpuStorage);
            return true;
        }

        PersistentWeakPointer<Graphics> graphics;
        Bool initialized;
        StandardAttributeSet enabledAttributes;
        UInt32 vertexCount;
        Bool indexed;
        UInt32 indexCount;
        Box3 boundingBox;

        AttributeArray<Point3rs>* vertexPositions;
        AttributeArray<Vector3rs>* vertexNormals;
        AttributeArray<Vector3rs>* vertexFaceNormals;
        AttributeArray<ColorS>* vertexColors;
        AttributeArray<Vector2rs>* vertexUVs0;
        std::shared_ptr<IndexBuffer> indexBuffer;

        // maps vertices to other equal vertices
        std::vector<UInt32>** vertexCrossMap;
        Bool shoudCalculateNormals;
        Real normalsSmoothingThreshold;

    };
}
