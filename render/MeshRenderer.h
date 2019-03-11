#pragma once

#include <memory>
#include <vector>

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
        virtual Bool forwardRender(const ViewDescriptor& viewDescriptor, const std::vector<WeakPointer<Light>>& lights) override;
        virtual Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, const std::vector<WeakPointer<Light>>& lights) override;
        virtual Bool supportsRenderPath(RenderPath renderPath) override;
        void setMaterial(WeakPointer<Material> material);

    private:
        MeshRenderer(WeakPointer<Graphics> graphics, WeakPointer<Material> material, WeakPointer<Object3D> owner);
        void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute,
                                        WeakPointer<AttributeArrayBase> array);
        void drawMesh(WeakPointer<Mesh> mesh);

        PersistentWeakPointer<Material> material;
    };
}
