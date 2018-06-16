#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    Mesh::Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, Bool indexed): graphics(graphics), initialized(false), vertexCount(vertexCount), indexed(indexed) {
        this->vertexPositions = nullptr;
        this->vertexColors = nullptr;
        this->vertexUVs = nullptr;
        this->indexBuffer = nullptr;
        initAttributes();
    }

    Mesh::~Mesh() {
        if (this->vertexPositions) {
            delete this->vertexPositions;
            this->vertexPositions = nullptr;
        }
        if (this->vertexColors) {
            delete this->vertexColors;
            this->vertexColors = nullptr;
        }
        if (this->vertexUVs) {
            delete this->vertexUVs;
            this->vertexUVs = nullptr;
        }
    }

    void Mesh::init() {
        if (indexed) this->initIndices();
        initialized = true;
    }

    UInt32 Mesh::getVertexCount() const {
        return this->vertexCount;
    }

    void Mesh::initAttributes() {
        for (UInt32 i = 0; i < (UInt32)StandardAttributes::_Count; i++) {
            this->disableAttribute((StandardAttributes)i);
        }
    }

    void Mesh::enableAttribute(StandardAttributes attribute) {
        this->enabledAttributes[(UInt32)attribute] = true;
    }

    void Mesh::disableAttribute(StandardAttributes attribute) {
        this->enabledAttributes[(UInt32)attribute] = false;
    }

    Bool Mesh::isAttributeEnabled(StandardAttributes attribute) {
        return this->enabledAttributes[(UInt32)attribute];
    }

    Bool Mesh::isIndexed() {
        return this->indexed;
    }

    void Mesh::calculateBoundingBox() {
        Vector3r min;
        Vector3r max;

        AttributeArray<Vector3rs>* vertexPositions = this->getVertexPositions();

        if (vertexPositions && this->isAttributeEnabled(StandardAttributes::Position)) {
            UInt32 index = 0;
            for (auto itr = vertexPositions->begin(); itr != vertexPositions->end(); ++itr) {
                Vector3rs& position = *itr;

                if (index == 0 || position.x < min.x) min.x = position.x;
                if (index == 0 || position.y < min.y) min.y = position.y;
                if (index == 0 || position.z < min.z) min.z = position.z;

                if (index == 0 || position.x > max.x) max.x = position.x;
                if (index == 0 || position.y > max.y) max.y = position.y;
                if (index == 0 || position.z > max.z) max.z = position.z;
                index++;
            }
        }

        this->boundingBox.setMin(min);
        this->boundingBox.setMax(max);
    }

    const Box3& Mesh::getBoundingBox() const {
        return this->boundingBox;
    }

 
    AttributeArray<Vector3rs>* Mesh::getVertexPositions() {
        return this->vertexPositions;
    }

    AttributeArray<ColorS>* Mesh::getVertexColors() {
        return this->vertexColors;
    }

    AttributeArray<Vector2rs>* Mesh::getVertexUVs() {
        return this->vertexUVs;
    }

    Bool Mesh::initVertexPositions(UInt32 vertexCount) {
        if (this->vertexPositions) delete this->vertexPositions;

        this->vertexPositions = new (std::nothrow) AttributeArray<Vector3rs>(vertexCount);
        if (this->vertexPositions == nullptr) {
            throw AllocationException("MeshGL::initVertexPositions() -> Unable to allocate positions array.");
        } 

        std::shared_ptr<AttributeArrayGPUStorage> gpuStorage = this->graphics->createGPUStorage(this->vertexPositions->getSize(), Vector3rs::ComponentCount, AttributeType::Float, false);
        this->vertexPositions->setGPUStorage(gpuStorage);

        return true;
    }

    Bool Mesh::initVertexColors(UInt32 vertexCount) {
        if (this->vertexColors) delete this->vertexColors;

        this->vertexColors = new (std::nothrow) AttributeArray<ColorS>(vertexCount);
        if (this->vertexColors == nullptr) {
            throw AllocationException("MeshGL::initVertexColors() -> Unable to allocate colors array.");
        }

        std::shared_ptr<AttributeArrayGPUStorage> gpuStorage = this->graphics->createGPUStorage(this->vertexPositions->getSize(), ColorS::ComponentCount, AttributeType::Float, false);
        this->vertexColors->setGPUStorage(gpuStorage);
        return true;
    }

    Bool Mesh::initVertexUVs(UInt32 vertexCount) {
        if (this->vertexUVs) delete this->vertexUVs;

        this->vertexUVs = new (std::nothrow) AttributeArray<Vector2rs>(vertexCount);
        if (this->vertexUVs == nullptr) {
            throw AllocationException("MeshGL::initVertexUVs() -> Unable to allocate uvs array.");
        }

        std::shared_ptr<AttributeArrayGPUStorage> gpuStorage = this->graphics->createGPUStorage(this->vertexPositions->getSize(), Vector2rs::ComponentCount, AttributeType::Float, false);
        this->vertexUVs->setGPUStorage(gpuStorage);
        return true;
    }

    Bool Mesh::initIndices() {
        this->indexBuffer = this->graphics->createIndexBuffer(vertexCount);
        return true;
    }

    std::shared_ptr<IndexBuffer> Mesh::getIndexBuffer() {
        return this->indexBuffer;
    }
}
