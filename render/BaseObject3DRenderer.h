#pragma once

#include <memory>
#include <vector>

#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"
#include "RenderPath.h"
#include "ViewDescriptor.h"
#include "BaseRenderable.h"
#include "../light/LightPack.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;

    class BaseObject3DRenderer : public Object3DComponent {
    public:
        BaseObject3DRenderer(WeakPointer<Object3D> owner) : Object3DComponent(owner), castShadows(true) {}
        virtual Bool forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        virtual Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        virtual Bool supportsRenderPath(RenderPath renderPath);
        virtual UInt32 getRenderQueueID() const = 0;
        virtual Bool init() = 0;
        Bool castsShadows();
        void setCastShadows(Bool castShadows);

    private:
        Bool castShadows;
    };
}
