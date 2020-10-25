#pragma once

#include "../common/types.h"

namespace Core {

    // forward declarations
    class Engine;

    class CoreObject {

        friend class Engine;
        
    public:
        UInt64 getObjectID() const;
        virtual ~CoreObject();

    protected:
        CoreObject();
        UInt64 objectID;
    };

}