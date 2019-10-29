#pragma once

#include <unordered_map>
#include <vector>

#include "../Engine.h"
#include "../common/types.h"
#include "../base/CoreObject.h"

namespace Core {

    class CoreObjectReferenceManager {
    public:

        enum class OwnerType {
            Single = 0,
            Multiple = 1
        };

        CoreObjectReferenceManager();
        ~CoreObjectReferenceManager();

        void addReference(std::shared_ptr<CoreObject> object, OwnerType ownerType);
        void removeReference(WeakPointer<CoreObject> object);
        UInt32 getReferenceCount(WeakPointer<CoreObject> object) const;
        UInt32 getReferenceCount(UInt64 id) const;

    private:
    
        std::unordered_map<UInt64, UInt32> referenceCounts;
        std::unordered_map<UInt64, UInt32> referenceIndex;
        std::unordered_map<UInt64, OwnerType> referenceOwnerTypes;
        std::vector<std::shared_ptr<CoreObject>> references;

    };

}