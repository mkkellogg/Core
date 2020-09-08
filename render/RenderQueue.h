#pragma once

#include <vector>

#include "RenderList.h"

namespace Core {

    class RenderQueue: public RenderList {
    public:

        RenderQueue(UInt32 id);
        UInt32 getID() const;

    private:

        UInt32 id;
    };
}