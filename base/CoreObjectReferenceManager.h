#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "../base/CoreObject.h"

namespace Core {

    class CoreObjectReferenceManager {
    public:

        enum class OwnerType {
            Single = 0,
            Multiple = 1
        };

        CoreObjectReferenceManager();
        virtual ~CoreObjectReferenceManager();

        void addReference(std::shared_ptr<CoreObject> object, OwnerType ownerType);
        void removeReference(WeakPointer<CoreObject> object);
        void addReferenceOwner(WeakPointer<CoreObject> object);
        void addReferenceOwner(UInt64 id);
        UInt32 getReferenceCount(WeakPointer<CoreObject> object) const;
        UInt32 getReferenceCount(UInt64 id) const;

    private:
    
        std::unordered_map<UInt64, UInt32> referenceCounts;
        std::unordered_map<UInt64, UInt32> referenceIndex;
        std::unordered_map<UInt64, OwnerType> referenceOwnerTypes;
        std::vector<std::shared_ptr<CoreObject>> references;

    };

}
