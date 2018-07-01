#pragma once

#include <memory>

#include "../material/StandardAttributes.h"
#include "../render/ObjectRenderer.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Engine;
    class Camera;
    class Material;
    class AttributeArrayBase;
    class Mesh;

    class MeshRenderer : public ObjectRenderer<Mesh> {
        friend class Engine;

    public:
        virtual void render(WeakPointer<Camera> camera) override;
        virtual void renderObject(WeakPointer<Camera> camera, WeakPointer<Mesh> mesh) override;

    private:
        MeshRenderer(WeakPointer<Graphics> graphics, WeakPointer<Material> material, WeakPointer<Object3D> owner);
        void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, StandardAttribute attribute, WeakPointer<AttributeArrayBase> array);

        PersistentWeakPointer<Material> material;
    };
}
