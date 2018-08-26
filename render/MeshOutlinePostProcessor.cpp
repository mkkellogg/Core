#include <memory>
#include <mutex>

#include "MeshOutlinePostProcessor.h"

namespace Core {

    void MeshOutlinePostProcessor::PostProcess() {
        
    }
    
    WeakPointer<MeshOutlinePostProcessor> MeshOutlinePostProcessor::instance() {
        if (!MeshOutlinePostProcessor::_instance) {
            std::lock_guard<std::mutex> lock(MeshOutlinePostProcessor::_instance_mutex);
            _instance = std::make_shared<MeshOutlinePostProcessor>();
        }
    }

}
