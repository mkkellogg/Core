#pragma once

#include "PostProcessor.h"

namespace Core {

    // forward declarations
    class Engine;

    class MeshOutlinePostProcessor: public PostProcessor {

        friend class Engine;

    public:

        void PostProcess() override;

    };

}