#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"
#include "../scene/Object3D.h"
#include "Vector3.h"
#include "IndexBuffer.h"
#include "IndexBuffer.h"
#include "../math/Math.h"
#include "../common/Constants.h"

namespace Core {

    Mesh::Mesh(WeakPointer<Graphics> graphics, UInt32 vertexCount, UInt32 indexCount): graphics(graphics), vertexCount(vertexCount), indexCount(indexCount) {
        this->vertexCrossMap = nullptr;
        this->initialized = false;
        this->indexed = indexCount > 0 ? true : false;
        this->enabledAttributes = StandardAttributes::createAttributeSet();
        this->normalsSmoothingThreshold = Math::PI / 2.0;
        this->shouldCalculateNormals = false;
        this->shouldCalculateTangents = false;
        this->shouldCalculateBounds = false;
        initAttributes();
    }

    Mesh::~Mesh() {
        this->destroyVertexCrossMap();
        if (this->indexBuffer.isValid()) {
            Engine::safeReleaseObject(this->indexBuffer);
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
        StandardAttributes::addAttribute(&this->enabledAttributes, attribute);
        if (attribute == StandardAttribute::Normal) {
            StandardAttributes::addAttribute(&this->enabledAttributes, StandardAttribute::AveragedNormal);
        }
        else if (attribute == StandardAttribute::AveragedNormal) {
            StandardAttributes::addAttribute(&this->enabledAttributes, StandardAttribute::Normal);
        }
    }

    void Mesh::disableAttribute(StandardAttribute attribute) {
        StandardAttributes::removeAttribute(&this->enabledAttributes, attribute);
        if (attribute == StandardAttribute::Normal) {
            StandardAttributes::removeAttribute(&this->enabledAttributes, StandardAttribute::AveragedNormal);
        }
        else if (attribute == StandardAttribute::AveragedNormal) {
            StandardAttributes::removeAttribute(&this->enabledAttributes, StandardAttribute::Normal);
        }
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
            UInt32 pointCount = vertexPositions->getAttributeCount();
            Point3rs * pos = vertexPositions->getAttributes();
            for (UInt32 i = 0; i < pointCount; i++) {
                Point3rs& position = *pos;
                if (i == 0 || position.x < min.x) min.x = position.x;
                if (i == 0 || position.y < min.y) min.y = position.y;
                if (i == 0 || position.z < min.z) min.z = position.z;

                if (i == 0 || position.x > max.x) max.x = position.x;
                if (i == 0 || position.y > max.y) max.y = position.y;
                if (i == 0 || position.z > max.z) max.z = position.z;
                pos++;
            }
        }

        this->boundingBox.setMin(min);
        this->boundingBox.setMax(max);
    }

    const Box3& Mesh::getBoundingBox() const {
        return this->boundingBox;
    }

    void Mesh::calculateBoundingSphere() {
        Point3r center;
        Vector3r centerToNewPoint;
        Vector3r centerToNewPointOffset;
        Real radius = 0.0f;
        if (vertexPositions && this->isAttributeEnabled(StandardAttribute::Position)) {
            UInt32 pointCount = vertexPositions->getAttributeCount();
            Point3rs * pos = vertexPositions->getAttributes();
            for (UInt32 i = 0; i < pointCount; i++) {
                Point3rs& position = *pos;
                if (i == 0) {
                    center = position;
                } else {
                    centerToNewPoint = position - center;
                    Real distanceToNewPoint = centerToNewPoint.magnitude();
                    if (distanceToNewPoint > radius) {
                        centerToNewPointOffset = centerToNewPoint;
                        centerToNewPointOffset.normalize();
                        Real halfDiff = (distanceToNewPoint - radius) * 0.5f;
                        centerToNewPointOffset = centerToNewPointOffset * halfDiff;
                        Real newRadius = radius + halfDiff;
                        center = center + centerToNewPointOffset;
                        radius = newRadius;
                    }
                }
                pos++;
            }
        }
        this->boundingSphere.set(center.x, center.y, center.z, radius);
    }

    const Vector4r& Mesh::getBoundingSphere() const {
        return this->boundingSphere;
    }

    WeakPointer<AttributeArray<Point3rs>> Mesh::getVertexPositions() {
        return this->vertexPositions;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexNormals() {
        return this->vertexNormals;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexAveragedNormals() {
        return this->vertexAveragedNormals;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexFaceNormals() {
        return this->vertexFaceNormals;
    }

    WeakPointer<AttributeArray<Vector3rs>> Mesh::getVertexTangents() {
        return this->vertexTangents;
    }

    WeakPointer<AttributeArray<ColorS>> Mesh::getVertexColors() {
        return this->vertexColors;
    }

    WeakPointer<AttributeArray<Vector2rs>> Mesh::getVertexAlbedoUVs() {
        return this->vertexAlbedoUVs;
    }

    WeakPointer<AttributeArray<Vector2rs>> Mesh::getVertexNormalUVs() {
        return this->vertexNormalUVs;
    }

    Bool Mesh::initVertexPositions() {
        return this->initVertexAttributes<Point3rs>(&this->vertexPositions, this->vertexCount);
    }

    Bool Mesh::initVertexNormals() {
        Bool result = true;
        result = this->initVertexAttributes<Vector3rs>(&this->vertexNormals, this->vertexCount);
        result = result && this->initVertexAttributes<Vector3rs>(&this->vertexAveragedNormals, this->vertexCount);
        return result;
    }

    Bool Mesh::initVertexFaceNormals() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexFaceNormals, this->vertexCount);
    }

    Bool Mesh::initVertexTangents() {
        return this->initVertexAttributes<Vector3rs>(&this->vertexTangents, this->vertexCount);
    }

    Bool Mesh::initVertexColors() {
        return this->initVertexAttributes<ColorS>(&this->vertexColors, this->vertexCount);
    }

    Bool Mesh::initVertexAlbedoUVs() {
        return this->initVertexAttributes<Vector2rs>(&this->vertexAlbedoUVs, this->vertexCount);
    }

    Bool Mesh::initVertexNormalUVs() {
        return this->initVertexAttributes<Vector2rs>(&this->vertexNormalUVs, this->vertexCount);
    }

    Bool Mesh::initIndices() {
        this->indexBuffer = Engine::instance()->createIndexBuffer(indexCount);
        return true;
    }

    WeakPointer<IndexBuffer> Mesh::getIndexBuffer() {
        return this->indexBuffer;
    }

    void Mesh::update() {
        if (this->shouldCalculateBounds) {
            this->calculateBoundingBox();
            this->calculateBoundingSphere();
        }
        if (this->shouldCalculateNormals){
            this->calculateNormals((Real)this->normalsSmoothingThreshold);
        }
        if (this->shouldCalculateTangents){
            this->calculateTangents((Real)this->normalsSmoothingThreshold);
        }
        //if (buildFaces)BuildFaces();

    }

    void Mesh::reverseVertexAttributeWindingOrder() {
        UInt32 realVertexCount = this->vertexCount;
        WeakPointer<IndexBuffer> indices;
        if (this->indexed) {
            indices = this->getIndexBuffer();
            realVertexCount = this->indexCount;
        }

        WeakPointer<AttributeArray<Vector3rs>> vertexNormals = this->vertexNormals;
        WeakPointer<AttributeArray<Vector3rs>> vertexAveragedNormals = this->vertexAveragedNormals;
        WeakPointer<AttributeArray<Vector3rs>> vertexFaceNormals = this->vertexFaceNormals;
        WeakPointer<AttributeArray<Point3rs>> vertexPositions = this->vertexPositions;
     
        for (UInt32 i = 0; i < realVertexCount; i+=3) {
            if (this->indexed) {

            }
            else {
                Point3rs& p2 = vertexPositions->getAttribute(i + 1);
                Point3rs& p3 = vertexPositions->getAttribute(i + 2);

                Point3r temp = p2;
                p2 = p3;
                p3 = temp;

                Vector3rs& n2 = vertexNormals->getAttribute(i + 1);
                Vector3rs& n3 = vertexNormals->getAttribute(i + 2);

                Vector3r tempV = n2;
                n2 = n3;
                n3 = tempV;

                Vector3rs& an2 = vertexAveragedNormals->getAttribute(i + 1);
                Vector3rs& an3 = vertexAveragedNormals->getAttribute(i + 2);

                tempV = an2;
                an2 = an3;
                an3 = tempV;

                Vector3rs& fn2 = vertexFaceNormals->getAttribute(i + 1);
                Vector3rs& fn3 = vertexFaceNormals->getAttribute(i + 2);

                tempV = fn2;
                fn2 = fn3;
                fn3 = tempV;
            }

            vertexPositions->updateGPUStorageData();
            vertexNormals->updateGPUStorageData();
            vertexAveragedNormals->updateGPUStorageData();
            vertexFaceNormals->updateGPUStorageData();

            this->update();

        }
    }

    void Mesh::setCalculateNormals(Bool calculateNormals) {
        this->shouldCalculateNormals = calculateNormals;
    }

    void Mesh::setCalculateTangents(Bool calculateTangents) {
        this->shouldCalculateTangents = calculateTangents;
    }

    void Mesh::setCalculateBounds(Bool calculateBoundingBox) {
        this->shouldCalculateBounds = calculateBoundingBox;
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
            this->buildVertexCrossMap();
        }

        UInt32 realVertexCount = this->vertexCount;
        WeakPointer<IndexBuffer> indices;
        if (this->indexed) {
            indices = this->getIndexBuffer();
            realVertexCount = this->indexCount;
        }

        WeakPointer<AttributeArray<Vector3rs>> vertexNormals = this->vertexNormals;
        WeakPointer<AttributeArray<Vector3rs>> vertexAveragedNormals = this->vertexAveragedNormals;
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

            this->vertexAveragedNormals->getAttribute(mappedIndex1).copy(normal);
            this->vertexAveragedNormals->getAttribute(mappedIndex2).copy(normal);
            this->vertexAveragedNormals->getAttribute(mappedIndex3).copy(normal);

            vertexFaceNormals->getAttribute(mappedIndex1).copy(normal);
            vertexFaceNormals->getAttribute(mappedIndex2).copy(normal);
            vertexFaceNormals->getAttribute(mappedIndex3).copy(normal);
        }

        // This vector is used to store the calculated average normal for all equal vertices
        // whose normals differ by an angle that is less than 'smoothingThreshhold'
        std::vector<Vector3r> averageNormals;

        // This vector is used to store the calculated average normal for all equal vertices
        std::vector<Vector3r> fullAverageNormals;

        // compute the cosine of the smoothing threshhold angle
        Real cosSmoothingThreshhold = (Math::cos(smoothingThreshhold));

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
            Vector3r fullAvg(0, 0, 0);
            Real fullDivisor = 0;

            std::vector<UInt32>& list = *listPtr;

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
                fullAvg.x += current.x;
                fullAvg.y += current.y;
                fullAvg.z += current.z;
                fullDivisor++;
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
            }
            averageNormals.push_back(avg);

            // if fullDivisor <= 1, then no valid normals were found to include in the average,
            // so just use the existing one
            if (fullDivisor <= 1) {
                fullAvg.x = oNormal.x;
                fullAvg.y = oNormal.y;
                fullAvg.z = oNormal.z;
            }
            else {
                Real scaleFactor = (Real)1.0 / fullDivisor;
                fullAvg.scale(scaleFactor);
            }

            fullAverageNormals.push_back(fullAvg);
        }

        // loop through each vertex and assign the average normal
        // calculated for that vertex
        for (UInt32 v = 0; v < realVertexCount; v++) {
            Vector3r avg = averageNormals[v];
            avg.normalize();
            Vector3r fullAvg = fullAverageNormals[v];
            fullAvg.normalize();
            
            UInt32 mappedIndex = v;
            if (this->indexed) {
                mappedIndex = indices->getIndex(mappedIndex);
            }

            // set the normal for this vertex to the averaged normal
            vertexNormals->getAttribute(mappedIndex).copy(avg);
            vertexAveragedNormals->getAttribute(mappedIndex).copy(fullAvg);
        }

        //if (invertNormals)InvertNormals(); 

        vertexNormals->updateGPUStorageData();
        vertexAveragedNormals->updateGPUStorageData();
        vertexFaceNormals->updateGPUStorageData();
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
        a = p3 - p1;
        b = p2 - p1;

        // calculate cross product
        Vector3r::cross(a, b, result);
        result.normalize();
    }

     /*
    * Calculate vertex tangents using the two incident edges of a given vertex.
    * For all triangles that share a given vertex,the method will
    * calculate the average tangent for that vertex as long as the angle between
    * the un-averaged normals for the same vertices is less than [smoothingThreshhold].
    * [smoothingThreshhold is specified in degrees.
    */
    void Mesh::calculateTangents(Real smoothingThreshhold) {
        if (!StandardAttributes::hasAttribute(this->enabledAttributes, StandardAttribute::Tangent)) return;

        WeakPointer<AttributeArray<Vector3rs>> tangents = this->getVertexTangents();
        WeakPointer<AttributeArray<Vector3rs>> faceNormals = this->getVertexFaceNormals();

        // loop through each triangle in this mesh's vertices
        // and calculate tangents for each
        for (UInt32 v = 0; v < this->vertexCount - 2; v += 3) {
            Vector3r t0, t1, t2;

            this->calculateTangent(v, v + 2, v + 1, t0);
            this->calculateTangent(v + 1, v, v + 2, t1);
            this->calculateTangent(v + 2, v + 1, v, t2);

            tangents->getAttribute(v).copy(t0);
            tangents->getAttribute(v + 1).copy(t1);
            tangents->getAttribute(v + 2).copy(t2);
        }

        // This vector is used to store the calculated average tangent for all equal vertices
        std::vector<Vector3r> averageTangents;

        // loop through each vertex and lookup the associated list of
        // tangents associated with that vertex, and then calculate the
        // average tangents from that list.
        for (UInt32 v = 0; v < this->vertexCount; v++) {
            // get existing normal for this vertex
            Vector3r oNormal;
            oNormal = faceNormals->getAttribute(v);
            oNormal.normalize();

            Vector3r oTangent;
            oTangent = vertexTangents->getAttribute(v);
            oTangent.normalize();

            // retrieve the list of equal vertices for vertex [v]
            std::vector<UInt32>* listPtr = vertexCrossMap[v];
            if (listPtr == nullptr) {
                throw Exception("Mesh::calculateTangents -> Null pointer to vertex group list.");
            }

            Vector3r avg(0, 0, 0);
            Real divisor = 0;

            std::vector<UInt32>& list = *listPtr;

            // compute the cosine of the smoothing threshhold angle
            Real cosSmoothingThreshhold = (Math::cos(smoothingThreshhold));

            for (UInt32 i = 0; i < list.size(); i++) {
                UInt32 vIndex = list[i];
                Vector3rs& current = faceNormals->getAttribute(vIndex);
                current.normalize();

                // calculate angle between the normal that exists for this vertex,
                // and the current normal in the list.
                Real dot = Vector3r::dot(current, oNormal);

                if (dot > cosSmoothingThreshhold) {
                    Vector3rs& tangent = tangents->getAttribute(vIndex);
                    avg.x += tangent.x;
                    avg.y += tangent.y;
                    avg.z += tangent.z;
                    divisor++;
                }
            }

            // if divisor < 1, then no extra tangents were found to include in the average,
            // so just use the original one

            if (divisor <= 1) {
                avg.x = oTangent.x;
                avg.y = oTangent.y;
                avg.z = oTangent.z;
            }
            else {
                Real scaleFactor = (Real)1.0 / divisor;
                avg.scale(scaleFactor);
                //avg.Normalize();
            }

            averageTangents.push_back(avg);
        }

        // loop through each vertex and assign the average tangent
        // calculated for that vertex
        for (UInt32 v = 0; v < this->vertexCount; v++) {
            Vector3r avg = averageTangents[v];
            avg.normalize();
            // set the tangent for this vertex to the averaged tangent
            tangents->getAttribute(v).set(avg.x, avg.y, avg.z);
        }

        //if (invertTangents)InvertTangents();

        tangents->updateGPUStorageData();
    }

    void Mesh::setName(const std::string& name) {
        this->name = name;
    }

    const std::string& Mesh::getName() const {
        return this->name;
    }

    /*
    * Calculate the tangent for the vertex at [vertexIndex] in member [positions].
    *
    * The two edges used in the calculation (e1 and e2) are formed from the three vertices: v0, v1, v2.
    *
    * v0 is the vertex at [vertexIndex] in [positions].
    * v2 is the vertex at [rightIndex] in [positions].
    * v1 is the vertex at [leftIndex] in [positions].
    */
    void Mesh::calculateTangent(UInt32 vertexIndex, UInt32 rightIndex, UInt32 leftIndex, Vector3r& result) {
        // if the mesh doesn't have UVs dedicated for normal mapping, use the albedo UVs as a backup
        WeakPointer<AttributeArray<Vector2rs>> sourceUVs = this->getVertexNormalUVs();
        if (!sourceUVs) sourceUVs = this->getVertexAlbedoUVs();
        Vector2r uv0 = sourceUVs->getAttribute(vertexIndex);
        Vector2r uv2 = sourceUVs->getAttribute(rightIndex);
        Vector2r uv1 = sourceUVs->getAttribute(leftIndex);

        WeakPointer<AttributeArray<Point3rs>> sourcePositions = this->getVertexPositions();
        Point3r p0 = sourcePositions->getAttribute(vertexIndex);
        Point3r p2 = sourcePositions->getAttribute(rightIndex);
        Point3r p1 = sourcePositions->getAttribute(leftIndex);

        Vector3r e1 = p1 - p0;
        Vector3r e2 = p2 - p0;

        Real u0, u1, u2;
        Real v0, v1, v2;

        u0 = uv0.x;
        u1 = uv1.x;
        u2 = uv2.x;

        v0 = uv0.y;
        v1 = uv1.y;
        v2 = uv2.y;

        Real du1 = u1 - u0;
        Real du2 = u2 - u0;

        Real dv1 = v1 - v0;
        Real dv2 = v2 - v0;

        Real ood = 1.0f / ((du1 * dv2) - (du2 * dv1));

        if (ood < 0) ood = -ood;

        result.set(dv2*e1.x - dv1 * e2.x, dv2*e1.y - dv1 * e2.y, dv2*e1.z - dv1 * e2.z);

        result.scale(ood);
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
