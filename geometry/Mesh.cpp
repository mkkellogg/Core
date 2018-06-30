#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../scene/Object3D.h"
#include "IndexBuffer.h"
#include "IndexBuffer.h"

namespace Core {

    Mesh::Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, UInt32 indexCount): graphics(graphics), initialized(false), vertexCount(vertexCount), indexCount(indexCount) {
        this->vertexPositions = nullptr;
        this->vertexNormals = nullptr;
        this->vertexColors = nullptr;
        this->vertexUVs0 = nullptr;
        this->indexBuffer = nullptr;
        this->indexed = indexCount > 0 ? true : false;
        initAttributes();
    }

    Mesh::~Mesh() {
        if (this->vertexPositions) {
            delete this->vertexPositions;
            this->vertexPositions = nullptr;
        }
        if (this->vertexNormals) {
            delete this->vertexNormals;
            this->vertexNormals = nullptr;
        }
        if (this->vertexColors) {
            delete this->vertexColors;
            this->vertexColors = nullptr;
        }
        if (this->vertexUVs0) {
            delete this->vertexUVs0;
            this->vertexUVs0 = nullptr;
        }
    }

    void Mesh::init() {
        if (indexed) this->initIndices();
        initialized = true;
    }

    UInt32 Mesh::getVertexCount() const {
        return this->vertexCount;
    }

    UInt32 Mesh::getIndexCount() const {
        return this->indexCount;
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

    AttributeArray<Vector3rs>* Mesh::getVertexNormals() {
        return this->vertexNormals;
    }

    AttributeArray<ColorS>* Mesh::getVertexColors() {
        return this->vertexColors;
    }

    AttributeArray<Vector2rs>* Mesh::getVertexUVs0() {
        return this->vertexUVs0;
    }

    Bool Mesh::initVertexPositions() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexPositions, this->vertexCount);
    }

    Bool Mesh::initVertexNormals() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexNormals, this->vertexCount);
    }

    Bool Mesh::initVertexColors() {
        return this->initVertexAttributes<ColorS>(&this->vertexColors, this->vertexCount);
    }

    Bool Mesh::initVertexUVs0() {
        return this->initVertexAttributes<Vector2rs>(&this->vertexUVs0, this->vertexCount);
    }

    Bool Mesh::initIndices() {
        this->indexBuffer = this->graphics->createIndexBuffer(indexCount);
        return true;
    }

    WeakPointer<IndexBuffer> Mesh::getIndexBuffer() {
        return this->indexBuffer;
    }

    /*
    * Calculate vertex normals using the two incident edges to calculate the
    * cross product. For all triangles that share a given vertex,the method will
    * calculate the average normal for that vertex as long as the angle between
    * the un-averaged normals is less than [smoothingThreshhold]. [smoothingThreshhold]
    * is specified in degrees.
    */
    /*void Mesh::calculateNormals(Real smoothingThreshhold) {
        if (!StandardAttributes::hasAttribute(this->, StandardAttribute::Normal))return;

        // loop through each triangle in this mesh's vertices
        // and calculate normals for each
        for (UInt32 v = 0; v < renderVertexCount - 2; v += 3) {
            Vector3 normal;
            CalculateFaceNormal(v, normal);

            vertexNormals.GetElement(v)->Set(normal.x, normal.y, normal.z);
            vertexNormals.GetElement(v + 1)->Set(normal.x, normal.y, normal.z);
            vertexNormals.GetElement(v + 2)->Set(normal.x, normal.y, normal.z);

            faceNormals.GetElement(v)->Set(normal.x, normal.y, normal.z);
            faceNormals.GetElement(v + 1)->Set(normal.x, normal.y, normal.z);
            faceNormals.GetElement(v + 2)->Set(normal.x, normal.y, normal.z);
        }

        // This vector is used to store the calculated average normal for all equal vertices
        std::vector<Vector3> averageNormals;

        // loop through each vertex and lookup the associated list of
        // normals associated with that vertex, and then calculate the
        // average normal from that list.
        for (UInt32 v = 0; v < renderVertexCount; v++) {
            // get existing normal for this vertex
            Vector3 oNormal;
            oNormal = *(faceNormals.GetElement(v));
            oNormal.Normalize();

            // retrieve the list of equal vertices for vertex [v]
            std::vector<UInt32>* listPtr = vertexCrossMap[v];
            NONFATAL_ASSERT(listPtr != nullptr, "SubMesh3D::CalculateNormals -> Null pointer to vertex group list.", true);

            Vector3 avg(0, 0, 0);
            Real divisor = 0;

            std::vector<UInt32>& list = *listPtr;

            // compute the cosine of the smoothing threshhold angle
            Real cosSmoothingThreshhold = (GTEMath::Cos(Constants::DegreesToRads * smoothingThreshhold));

            for (UInt32 i = 0; i < list.size(); i++) {
                UInt32 vIndex = list[i];
                Vector3 * currentPtr = faceNormals.GetElement(vIndex);
                Vector3 current = *currentPtr;
                current.Normalize();

                // calculate angle between the normal that exists for this vertex,
                // and the current normal in the list.
                Real dot = Vector3::Dot(current, oNormal);

                if (dot > cosSmoothingThreshhold) {
                    avg.x += current.x;
                    avg.y += current.y;
                    avg.z += current.z;
                    divisor++;
                }
            }

            // if divisor <= 1, then no valid normals were found to include in the average,
            // so just use the existing one
            if (divisor <= 1) {
                avg.x = oNormal.x;
                avg.y = oNormal.y;
                avg.z = oNormal.z;
            }
            else {
                Real scaleFactor = (Real)1.0 / divisor;
                avg.Scale(scaleFactor);
                //avg.Normalize();
            }

            averageNormals.push_back(avg);
        }

        // loop through each vertex and assign the average normal
        // calculated for that vertex
        for (UInt32 v = 0; v < renderVertexCount; v++) {
            Vector3 avg = averageNormals[v];
            avg.Normalize();
            // set the normal for this vertex to the averaged normal
            vertexNormals.GetElement(v)->Set(avg.x, avg.y, avg.z);
        }

        if (invertNormals)InvertNormals();
    }*/

}
