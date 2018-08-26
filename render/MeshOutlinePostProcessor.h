#pragma once

#include "PostProcessor.h"
#include "../util/WeakPointer.h"

namespace Core {

    // forward declarations
    class Engine;

    class MeshOutlinePostProcessor: public PostProcessor {

        friend class Engine;

    public:

        void PostProcess() override;
        static WeakPointer<MeshOutlinePostProcessor> instance();

    private:

        static std::shared_ptr<MeshOutlinePostProcessor> _instance;
        static std::mutex _instance_mutex;
    };

}