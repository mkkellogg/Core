#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../scene/Object3D.h"
#include "IndexBuffer.h"
#include "IndexBuffer.h"

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
        for (UInt32 i = 0; i < (UInt32)StandardAttribute::_Count; i++) {
            this->disableAttribute((StandardAttribute)i);
        }
    }

    void Mesh::enableAttribute(StandardAttribute attribute) {
        this->enabledAttributes[(UInt32)attribute] = true;
    }

    void Mesh::disableAttribute(StandardAttribute attribute) {
        this->enabledAttributes[(UInt32)attribute] = false;
    }

    Bool Mesh::isAttributeEnabled(StandardAttribute attribute) {
        return this->enabledAttributes[(UInt32)attribute];
    }

    Bool Mesh::isIndexed() {
        return this->indexed;
    }

    void Mesh::calculateBoundingBox() {
        Vector3r min;
        Vector3r max;

        AttributeArray<Vector3rs>* vertexPositions = this->getVertexPositions();

        if (vertexPositions && this->isAttributeEnabled(StandardAttribute::Position)) {
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

    Bool Mesh::initVertexPositions() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexPositions, this->vertexCount);
    }

    Bool Mesh::initVertexColors() {
        return this->initVertexAttributes<ColorS>(&this->vertexColors, this->vertexCount);
    }

    Bool Mesh::initVertexUVs() {
        return this->initVertexAttributes<Vector2rs>(&this->vertexUVs, this->vertexCount);
    }

    Bool Mesh::initIndices() {
        this->indexBuffer = this->graphics->createIndexBuffer(vertexCount);
        return true;
    }

    std::shared_ptr<IndexBuffer> Mesh::getIndexBuffer() {
        return this->indexBuffer;
    }
}
