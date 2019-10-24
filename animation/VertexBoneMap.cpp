
#include <memory.h>

#include "VertexBoneMap.h"
#include "../Engine.h"
#include "Skeleton.h"
#include "../geometry/Mesh.h"
#include "../common/assert.h"

namespace Core {
    /*
    * Only constructor, parameterized.
    */
    VertexBoneMap::VertexBoneMap(UInt32 vertexCount, UInt32 uVertexCount) {
        this->vertexCount = vertexCount;
        this->uniqueVertexCount = uVertexCount;
        this->mappingDescriptors = nullptr;
    }

    /*
     * Destructor.
     */
    VertexBoneMap::~VertexBoneMap() {
        this->destroy();
    }

    /*
     * Destroy this VertexBoneMap object.
     */
    void VertexBoneMap::destroy() {
        // delete [mappingDescriptors] and set to nullptr
        if (this->mappingDescriptors != nullptr) {
            delete[] mappingDescriptors;
           this-> mappingDescriptors = nullptr;
        }
    }

    /*
     * Initialize this VertexBoneMap object. This method MUST be called before this object can be used.
     */
    Bool VertexBoneMap::init() {
        // destroy existing data, if it exists
        this->destroy();

        this->mappingDescriptors = new(std::nothrow) VertexMappingDescriptor[vertexCount];
        if (this->mappingDescriptors == nullptr) {
            throw AllocationException("VertexBoneMap::init -> unable to allocate vertex mapping descriptors master array.");
        }

        return true;
    }

    /*
     * Get VertexMappingDescriptor for vertex (non-unique) at [index]
     */
    VertexBoneMap::VertexMappingDescriptor* VertexBoneMap::getDescriptor(UInt32 index) {
        if (index >= this->vertexCount) {
            throw OutOfRangeException("VertexBoneMap::getDescriptor -> Index out of range.");
        }

        return mappingDescriptors + index;
    }

    /*
     * Get total number of vertices that are mapped.
     */
    UInt32 VertexBoneMap::getVertexCount() const {
        return this->vertexCount;
    }

    /*
     * Get number of unique vertices that are mapped.
     */
    UInt32 VertexBoneMap::getUniqueVertexCount() const {
        return this->uniqueVertexCount;
    }

// virtual std::shared_ptr<AttributeArrayGPUStorage> Graphics::createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const = 0;
    void VertexBoneMap::buildAttributeArray() {
        try {
            this->boneWeights = std::make_shared<AttributeArray<Vector4rs>>(this->vertexCount);
            this->boneIndices = std::make_shared<AttributeArray<Vector4us>>(this->vertexCount);
        } catch(...) {
            throw AllocationException("VertexBoneMap::buildAttributeArray() -> Unable to allocate array.");
        }

        WeakPointer<AttributeArrayGPUStorage> boneWeightsGpuStorage =
            Engine::instance()->getGraphicsSystem()->createGPUStorage(this->boneWeights->getSize(), 4, AttributeType::Float, false);
        this->boneWeights->setGPUStorage(boneWeightsGpuStorage);


        WeakPointer<AttributeArrayGPUStorage> boneIndicesGpuStorage =
            Engine::instance()->getGraphicsSystem()->createGPUStorage(this->boneIndices->getSize(), 4, AttributeType::UnsignedInt, false);
        this->boneIndices->setGPUStorage(boneIndicesGpuStorage);

        Real* boneWeightsDataArray = new (std::nothrow) Real[this->vertexCount * Constants::MaxBonesPerVertex];
        if (boneWeightsDataArray == nullptr) {
            throw AllocationException("VertexBoneMap::buildAttributeArray() -> Unable to allocate bone weights raw data storage.");
        }
        std::unique_ptr<Real[]> spBoneWeights(boneWeightsDataArray);

        UInt32* boneIndicesDataArray = new (std::nothrow) UInt32[this->vertexCount * Constants::MaxBonesPerVertex];
        if (boneIndicesDataArray == nullptr) {
            throw AllocationException("VertexBoneMap::buildAttributeArray() -> Unable to allocate bone indices raw data storage.");
        }
        std::unique_ptr<UInt32[]> spBoneIndices(boneIndicesDataArray);

        for (UInt32 i = 0; i < this->vertexCount; i++) {
            VertexMappingDescriptor* desc = this->getDescriptor(i);
            UInt32 baseIndex = i * Constants::MaxBonesPerVertex;
            
            for (UInt32 b = 0; b < Constants::MaxBonesPerVertex; b++) {
                boneWeightsDataArray[baseIndex + b] = desc->Weight[b];
                boneIndicesDataArray[baseIndex + b] = desc->BoneIndex[b];
            }
        }

        this->boneWeights->store(boneWeightsDataArray);
        this->boneIndices->store(boneIndicesDataArray);
    }

    WeakPointer<AttributeArray<Vector4rs>> VertexBoneMap::getWeights() {
        return this->boneWeights;
    }

    WeakPointer<AttributeArray<Vector4us>> VertexBoneMap::getIndices() {
        return this->boneIndices;
    }

    /*
     * Update the bone indices in this map to match that of [skeleton]
     */
    void VertexBoneMap::bindTo(WeakPointer<const Skeleton> skeleton) {
        if(!skeleton.isValid()) {
            throw InvalidReferenceException("VertexBoneMap::bindTo -> 'skeleton' is not valid.");
        }

        for (UInt32 v = 0; v < vertexCount; v++) {
            VertexBoneMap::VertexMappingDescriptor * desc = this->getDescriptor(v);
            for (UInt32 b = 0; b < desc->BoneCount; b++) {
                Int32 boneIndex = skeleton->getBoneMapping(desc->Name[b]);
                desc->BoneIndex[b] = boneIndex;
            }
        }
    }

    /*
     * Create a full (deep) clone of this VertexBoneMap object.
     */
    VertexBoneMap * VertexBoneMap::fullClone() {
        // allocate new VertexBoneMap objects
        VertexBoneMap * clone = new(std::nothrow) VertexBoneMap(vertexCount, uniqueVertexCount);
        if (clone == nullptr) {
            throw AllocationException("VertexBoneMap::FullClone -> Could not allocate vertex bone map.");
        }

        // initialize the new map
        Bool initSuccess = clone->init();
        if(!initSuccess) {
            throw Exception("VertexBoneMap::fullClone -> Could not initialize vertex bone map.");
        }

        // copy over VertexMappingDescriptors one-by-one
        for (UInt32 v = 0; v < vertexCount; v++) {
            clone->getDescriptor(v)->copy(this->getDescriptor(v));
        }

        return clone;
    }
}
