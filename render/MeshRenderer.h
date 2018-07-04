#pragma once

#include <memory>
#include <vector>

#include "../render/ObjectRenderer.h"
#include "../material/StandardAttributes.h"
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
        virtual void render(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights) override;
        virtual void renderObject(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, const std::vector<WeakPointer<Light>>& lights) override;

    private:
        MeshRenderer(WeakPointer<Graphics> graphics, WeakPointer<Material> material, WeakPointer<Object3D> owner);
        void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute, WeakPointer<AttributeArrayBase> array);
        void drawMesh(WeakPointer<Mesh> mesh);

        PersistentWeakPointer<Material> material;
    };
}
