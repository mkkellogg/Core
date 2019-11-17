/*********************************************
*
* class: VertexBoneMap
*
* author: Mark Kellogg
*
* This class stores vertex skinning information. Specifically it maps mesh vertices
* to instances of VertexMappingDescriptor, which contain information about what bone(s)
* the vertices are attached to and the respective weight for each attachment.
*
***********************************************/

#pragma once

#include <memory.h>

#include "../base/CoreObject.h"
#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../common/assert.h"
#include "../common/Constants.h"
#include "../geometry/Vector4.h"

namespace Core {

    // forward declarations
    class Skeleton;
    template <typename T> class AttributeArray;

    class VertexBoneMap : public CoreObject {
    public:

        friend class Engine;

        // this class holds vertex skinning information for a single vertex
        class VertexMappingDescriptor {
        public:

            // the unique vertex index, all vertices that have the same x,y,z values will have
            // the same UVertexIndex value
            UInt32 UniqueVertexIndex = 0;
            // number of bones to which this vertex is attached
            UInt32 BoneCount = 0;
            // bones to which this vertex is attached
            UInt32 BoneIndex[Constants::MaxBonesPerVertex];
            // weight of each bone attachment, should add up to 1
            Real Weight[Constants::MaxBonesPerVertex];
            // name of the bones, used for rebind to different skeletons
            std::string Name[Constants::MaxBonesPerVertex];

            // make this VertexMappingDexcriptor object identical to [desc].
            void copy(VertexMappingDescriptor* desc) {
                if (desc == nullptr) {
                    throw NullPointerException("VertexMappingDescriptor::SetTo -> 'desc' is null.");
                }
                
                this->BoneCount = desc->BoneCount;
                this->UniqueVertexIndex = desc->UniqueVertexIndex;
                memcpy(this->BoneIndex, desc->BoneIndex, sizeof(UInt32) * Constants::MaxBonesPerVertex);
                memcpy(this->Weight, desc->Weight, sizeof(Real) * Constants::MaxBonesPerVertex);
                for (UInt32 b = 0; b < Constants::MaxBonesPerVertex; b++) {
                    Name[b] = desc->Name[b];
                }
            }
        };

        virtual ~VertexBoneMap();
        Bool init();
        VertexMappingDescriptor* getDescriptor(UInt32 index);
        UInt32 getVertexCount() const;
        UInt32 getUniqueVertexCount() const;
        void buildAttributeArray();
        void bindTo(WeakPointer<const Skeleton> skeleton);

        WeakPointer<AttributeArray<Vector4rs>> getWeights();
        WeakPointer<AttributeArray<Vector4is>> getIndices();

    protected:

        VertexBoneMap(UInt32 vertexCount, UInt32 uVertexCount);


    private:

        // number of unique vertices
        UInt32 uniqueVertexCount;
        // total number of vertices
        UInt32 vertexCount;
        // mapping descriptor for each vertex
        VertexMappingDescriptor * mappingDescriptors;

        std::shared_ptr<AttributeArray<Vector4rs>> boneWeights;
        std::shared_ptr<AttributeArray<Vector4is>> boneIndices;

        void destroy();
        //VertexBoneMap * fullClone();
    };
}
