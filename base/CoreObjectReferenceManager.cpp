#include "CoreObjectReferenceManager.h"

namespace Core {

    CoreObjectReferenceManager::CoreObjectReferenceManager() {
    }

    CoreObjectReferenceManager::~CoreObjectReferenceManager() {
    }

    void CoreObjectReferenceManager::addReference(std::shared_ptr<CoreObject> object, OwnerType ownerType) {
        const UInt64 objectID = object->getObjectID();
        const UInt32 currentReferenceCount = this->getReferenceCount(objectID);
        this->referenceCounts[objectID] = currentReferenceCount;
        this->referenceOwnerTypes[objectID] = ownerType;
        this->referenceIndex[objectID] = this->references.size();
        this->references.push_back(object);
    }

    void CoreObjectReferenceManager::removeReference(WeakPointer<CoreObject> object) {
        const UInt32 objectID = object->getObjectID();
        std::unordered_map<UInt64, UInt32>::const_iterator loc = this->referenceCounts.find(objectID);
        if (loc != this->referenceCounts.end()) {
            UInt32& referenceCount = this->referenceCounts[objectID];
            if (referenceCount == 0) {
                throw Exception("CoreObjectReferenceManager::removeReference() -> Reference count is already zero.");
            }
            referenceCount--;
            if (referenceCount == 0) {
                this->references.erase(this->references.begin() + this->referenceIndex[objectID]);
            }
        } else {
            throw Exception("CoreObjectReferenceManager::removeReference() -> 'object' not present.");
        }
    }

    UInt32 CoreObjectReferenceManager::getReferenceCount(WeakPointer<CoreObject> object) const {
        return getReferenceCount(object->getObjectID());
    }

    UInt32 CoreObjectReferenceManager::getReferenceCount(UInt64 id) const {
        std::unordered_map<UInt64, UInt32>::const_iterator loc = this->referenceCounts.find(id);
        if (loc != this->referenceCounts.end()) {
            return (*loc).second;
        }
        return 0;
    }
}