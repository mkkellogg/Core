#pragma once

#include <memory>
#include <vector>

#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;
    class Light;
    class RenderTargetCube;

    class ReflectionProbe : public Object3DComponent {
    public:
        ReflectionProbe(WeakPointer<Object3D> owner);
        void init();
        void setNeedsUpdate(Bool needsUpdate);
        Bool getNeedsUpdate();
        void update();

    private:
        Bool needsUpdate;
        WeakPointer<RenderTargetCube> renderTarget;
    };
}
