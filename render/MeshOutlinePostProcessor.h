#pragma once

#include <memory>
#include <mutex>

#include "PostProcessor.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget2D;

    class MeshOutlinePostProcessor: public PostProcessor {

        friend class Engine;

    public:

        void PostProcess() override;
        static WeakPointer<MeshOutlinePostProcessor> instance();

    private:
        void init();
        void initializeRenderTarget();

        PersistentWeakPointer<RenderTarget2D> renderTarget;
        static std::shared_ptr<MeshOutlinePostProcessor> _instance;
        static std::mutex _instance_mutex;
    };

}