#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../scene/Object3D.h"
#include "IndexBuffer.h"
#include "IndexBuffer.h"

namespace Core {

    Mesh::Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, UInt32 indexCount): graphics(graphics), vertexCount(vertexCount), indexCount(indexCount) {
        this->vertexCrossMap = nullptr;
        this->initialized = false;
        this->indexed = indexCount > 0 ? true : false;
        this->enabledAttributes = StandardAttributes::createAttributeSet();
        this->normalsSmoothingThreshold = Math::PI / 2.0;
        this->shoudCalculateNormals = false;
        initAttributes();
    }

    Mesh::~Mesh() {
        this->destroyVertexCrossMap();
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
        StandardAttributes::addAttribute(&this->enabledAttributes, attribute);
    }

    void Mesh::disableAttribute(StandardAttribute attribute) {
        StandardAttributes::removeAttribute(&this->enabledAttributes, attribute);
    }

    Bool Mesh::isAttributeEnabled(StandardAttribute attribute) {
        return StandardAttributes::hasAttribute(this->enabledAttributes, attribute);
    }

    Bool Mesh::isIndexed() {
        return this->indexed;
    }

    void Mesh::calculateBoundingBox() {
        Vector3r min;
        Vector3r max;

        WeakPointer<AttributeArray<Point3rs>> vertexPositions = this->vertexPositions;

        if (vertexPositions && this->isAttributeEnabled(StandardAttribute::Position)) {
            UInt32 index = 0;
            for (auto itr = vertexPositions->begin(); itr != vertexPositions->end(); ++itr) {
                Point3rs& position = *itr;

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

    WeakPointer<AttributeArray<Point3rs>> Mesh::getVertexPositions() {
        return this->vertexPositions;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexNormals() {
        return this->vertexNormals;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexFaceNormals() {
        return this->vertexFaceNormals;
    }

    WeakPointer<AttributeArray<ColorS>> Mesh::getVertexColors() {
        return this->vertexColors;
    }

    WeakPointer<AttributeArray<Vector2rs>> Mesh::getVertexUVs0() {
        return this->vertexUVs0;
    }

    Bool Mesh::initVertexPositions() {
        return this->initVertexAttributes<Point3rs>(&this->vertexPositions, this->vertexCount);
    }

    Bool Mesh::initVertexNormals() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexNormals, this->vertexCount);
    }

    Bool Mesh::initVertexFaceNormals() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexFaceNormals, this->vertexCount);
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

    void Mesh::update() {
        if (this->shoudCalculateNormals /*|| calculateTangents || buildFaces*/) {
            if (!this->buildVertexCrossMap())return;
        }

        //if (calculateBoundingBox)CalculateBoundingBox();
        if (this->shoudCalculateNormals){
            this->calculateNormals((Real)this->normalsSmoothingThreshold);
        }
       // if (calculateTangents)CalculateTangents((Real)normalsSmoothingThreshold);
        //if (buildFaces)BuildFaces();

    }

    void Mesh::setCalculateNormals(Bool calculateNormals) {
        this->shoudCalculateNormals = calculateNormals;
    }

    void Mesh::setNormalsSmoothingThreshold(Real threshold) {
        this->normalsSmoothingThreshold = threshold;
    }

    /*
    * Calculate vertex normals using the two incident edges to calculate the
    * cross product. For all triangles that share a given vertex,the method will
    * calculate the average normal for that vertex as long as the angle between
    * the un-averaged normals is less than [smoothingThreshhold]. [smoothingThreshhold]
    * is specified in radians.
    */
    void Mesh::calculateNormals(Real smoothingThreshhold) {
        if (!StandardAttributes::hasAttribute(this->enabledAttributes, StandardAttribute::Normal))return;

        if (this->vertexCrossMap == nullptr) {
            throw Exception("Mesh::calculateNormal() -> 'vertexCrossMap' is null.");
        }

        UInt32 realVertexCount = this->vertexCount;
        WeakPointer<IndexBuffer> indices;
        if (this->indexed) {
            indices = this->getIndexBuffer();
            realVertexCount = this->indexCount;
        }

        WeakPointer<AttributeArray<Vector3rs>> vertexNormals = this->vertexNormals;
        WeakPointer<AttributeArray<Vector3rs>> vertexFaceNormals = this->vertexFaceNormals;
     
        // loop through each triangle in this mesh's vertices
        // and calculate normals for each
        for (UInt32 v = 0; v < realVertexCount - 2; v += 3) {
            Vector3r normal;
            this->calculateFaceNormal(v, normal);

            UInt32 mappedIndex1 = v;
            UInt32 mappedIndex2 = v + 1;
            UInt32 mappedIndex3 = v + 2;
            if (this->indexed) {
                mappedIndex1 = indices->getIndex(mappedIndex1);
                mappedIndex2 = indices->getIndex(mappedIndex2);
                mappedIndex3 = indices->getIndex(mappedIndex3);
            }

            vertexNormals->getAttribute(mappedIndex1).copy(normal);
            vertexNormals->getAttribute(mappedIndex2).copy(normal);
            vertexNormals->getAttribute(mappedIndex3).copy(normal);

            vertexFaceNormals->getAttribute(mappedIndex1).copy(normal);
            vertexFaceNormals->getAttribute(mappedIndex2).copy(normal);
            vertexFaceNormals->getAttribute(mappedIndex3).copy(normal);
        }

        // This vector is used to store the calculated average normal for all equal vertices
        std::vector<Vector3r> averageNormals;

        // loop through each vertex and lookup the associated list of
        // normals associated with that vertex, and then calculate the
        // average normal from that list.
        for (UInt32 v = 0; v < realVertexCount; v++) {

            UInt32 mappedIndex = v;
            if (this->indexed) {
                mappedIndex = indices->getIndex(mappedIndex);
            }

            // get existing normal for this vertex
            Vector3r oNormal;
            oNormal = vertexFaceNormals->getAttribute(mappedIndex);
            oNormal.normalize();

            // retrieve the list of equal vertices for vertex [v]
            std::vector<UInt32>* listPtr = vertexCrossMap[v];
            if (listPtr == nullptr) {
                throw Exception("Mesh::calculateNormals -> Null pointer to vertex group list.");
            }

            Vector3r avg(0, 0, 0);
            Real divisor = 0;

            std::vector<UInt32>& list = *listPtr;

            // compute the cosine of the smoothing threshhold angle
            Real cosSmoothingThreshhold = (Math::cos(smoothingThreshhold));

            for (UInt32 i = 0; i < list.size(); i++) {
                UInt32 vIndex = list[i];

                UInt32 mappedSubIndex = vIndex;
                if (this->indexed) {
                    mappedSubIndex = indices->getIndex(mappedSubIndex);
                }

                Vector3r current = vertexFaceNormals->getAttribute(mappedSubIndex);
                current.normalize();

                // calculate angle between the normal that exists for this vertex,
                // and the current normal in the list.
                Real dot = Vector3r::dot(current, oNormal);

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
                avg.scale(scaleFactor);
                //avg.Normalize();
            }

            averageNormals.push_back(avg);
        }

        // loop through each vertex and assign the average normal
        // calculated for that vertex
        for (UInt32 v = 0; v < realVertexCount; v++) {
            Vector3r avg = averageNormals[v];
            avg.normalize();
            
            UInt32 mappedIndex = v;
            if (this->indexed) {
                mappedIndex = indices->getIndex(mappedIndex);
            }

            // set the normal for this vertex to the averaged normal
            vertexNormals->getAttribute(mappedIndex).copy(avg);
        }

        //if (invertNormals)InvertNormals(); 

        vertexNormals->updateGPUStorageData();
    }

    /*
     * For a given face in the sub-mesh specified by [faceVertexIndex], calculate the face's
     * normal and store the result in [result]. [faceVertexIndex] will be the index of the
     * face's first vertex in [vertexPositions], the next two will be at [faceVertexIndex] + 1,
     * and [faceVertexIndex] + 2.
     */
    void Mesh::calculateFaceNormal(UInt32 faceVertexIndex, Vector3r& result) {
        UInt32 realVertexCount = this->vertexCount;
        WeakPointer<IndexBuffer> indices;
        if (this->indexed) {
            indices = this->getIndexBuffer();
            realVertexCount = this->indexCount;
        }
        if (faceVertexIndex >= realVertexCount - 2) {
            throw Exception("Mesh::calculateFaceNormal -> 'faceIndex' is out range.");
        }

        UInt32 mappedFaceIndex1 = faceVertexIndex;
        UInt32 mappedFaceIndex2 = faceVertexIndex + 1;
        UInt32 mappedFaceIndex3 = faceVertexIndex + 2;
        if (this->indexed) {
            mappedFaceIndex1 = indices->getIndex(mappedFaceIndex1);
            mappedFaceIndex2 = indices->getIndex(mappedFaceIndex2);
            mappedFaceIndex3 = indices->getIndex(mappedFaceIndex3);
        }
     
        Vector3r a, b;

        WeakPointer<AttributeArray<Point3rs>> positions = this->vertexPositions;
       
        Point3r p1 = positions->getAttribute(mappedFaceIndex1);
        Point3r p2 = positions->getAttribute(mappedFaceIndex2);
        Point3r p3 = positions->getAttribute(mappedFaceIndex3);

        // form 2 vectors based on triangle's vertices
        a = p2 - p1;
        b = p3 - p1;

        // calculate cross product
        Vector3r::cross(a, b, result);
        result.normalize();
    }

    /*
     * Deallocate and destroy [vertexCrossMap].
     */
    void Mesh::destroyVertexCrossMap() {
        if (this->vertexCrossMap != nullptr) {
            UInt32 realVertexCount = this->vertexCount;
            if (this->indexed) {
                realVertexCount = this->indexCount;
            }

            std::unordered_map<std::vector<UInt32>*, Bool> deleted;
            for (UInt32 i = 0; i < realVertexCount; i++) {
                std::vector<UInt32>* list = this->vertexCrossMap[i];
                if (list != nullptr && !deleted[list]) {
                    delete list;
                    deleted[list] = true;
                }
            }

            delete[] this->vertexCrossMap;
            this->vertexCrossMap = nullptr;
        }
    }

    /*
     * Construct [vertexCrossMap]. The vertex cross map is used to group all vertices that are equal.
     * For a given combination of x,y,z, a corresponding list exists in [vertexCrossMap] with all the indices
     * of vertices in [vertexPositions] which have a matching value for x,y, and z.
     */
    Bool Mesh::buildVertexCrossMap() {
        // destroy existing cross map (if there is one).
        this->destroyVertexCrossMap();

        // This map is used to link all equal vertices. Many triangles in a mesh can potentially have equal
        // vertices, so this structure is used to store indices in [vertexPositions] for those vertices.
        std::unordered_map<Point3r, std::vector<UInt32>*, Vector3Base<Real>::Hasher, Vector3Base<Real>::Eq> vertexGroups;

        UInt32 realVertexCount = this->vertexCount;
        WeakPointer<IndexBuffer> indices;
        if (this->indexed) {
            indices = this->getIndexBuffer();
            realVertexCount = this->indexCount;
        }

        vertexCrossMap = new(std::nothrow) std::vector<UInt32>*[realVertexCount];
        if (vertexCrossMap == nullptr) {
            throw AllocationException("Mesh::buildVertexCrossMap -> Could not allocate vertexCrossMap.");
        }
        
        // loop through each vertex in the mesh and add the index in [vertexPositions] for that vertex to the
        // appropriate vertex group.
        for (UInt32 v = 0; v < realVertexCount; v++) {
            UInt32 mappedIndex = v;
            if (this->indexed) {
                mappedIndex = indices->getIndex(mappedIndex);
            }

            Point3r point = this->vertexPositions->getAttribute(mappedIndex);

            std::vector<UInt32>*& list = vertexGroups[point];

            if (list == nullptr)list = new(std::nothrow) std::vector<UInt32>();
            if (list == nullptr) {
                throw AllocationException("Mesh::buildVertexCrossMap -> Could not allocate sub list.");
            }

            // add the normal at index [v] to the vertex group linked to [point]
            list->push_back(v);
            vertexCrossMap[v] = list;
        }

        return true;
    }
}
