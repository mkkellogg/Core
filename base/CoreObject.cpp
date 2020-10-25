#include "CoreObject.h"
#include <atomic> 

static std::atomic<Core::UInt64> _cur_id;

namespace Core {

    CoreObject::CoreObject() {
        this->objectID = _cur_id++;
    }

    CoreObject::~CoreObject() {
        
    }

    UInt64 CoreObject::getObjectID() const {
        return this->objectID;
    }

}