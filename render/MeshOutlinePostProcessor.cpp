#include "MeshOutlinePostProcessor.h"
#include "../image/TextureAttr.h"
#include "../common/Constants.h"
#include "../geometry/Vector2.h"
#include "../Engine.h"
#include "../Graphics.h"

namespace Core {

    void MeshOutlinePostProcessor::PostProcess() {
        
    }

    void MeshOutlinePostProcessor::init() {
        this->initializeRenderTarget();
    }
    
    WeakPointer<MeshOutlinePostProcessor> MeshOutlinePostProcessor::instance() {
        if (!MeshOutlinePostProcessor::_instance) {
            std::lock_guard<std::mutex> lock(MeshOutlinePostProcessor::_instance_mutex);
            if (!MeshOutlinePostProcessor::_instance) {
                MeshOutlinePostProcessor::_instance = std::make_shared<MeshOutlinePostProcessor>();
                MeshOutlinePostProcessor::_instance->init();
            }
        }
        return MeshOutlinePostProcessor::_instance;
    }

    void MeshOutlinePostProcessor::initializeRenderTarget() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        TextureAttributes colorTextureAttributes;
        colorTextureAttributes.Format = TextureFormat::RGBA8;
        colorTextureAttributes.FilterMode = TextureFilter::BiLinear;
        colorTextureAttributes.WrapMode = TextureWrap::Border;
        colorTextureAttributes.BorderWrapColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
        TextureAttributes depthTextureAttributes;
        depthTextureAttributes.FilterMode = TextureFilter::Point;
        depthTextureAttributes.WrapMode = TextureWrap::Border;
        depthTextureAttributes.BorderWrapColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
        Vector2u renderTargetSize(Constants::TempRenderTargetSize, Constants::TempRenderTargetSize);
        this->renderTarget = graphics->createRenderTarget2D(true, true, false, colorTextureAttributes,
                                                                      depthTextureAttributes, renderTargetSize);

    }

}
