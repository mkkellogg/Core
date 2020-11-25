#pragma once

#include <memory>
#include <vector>

#include "../material/StandardAttributes.h"
#include "../render/BaseRenderable.h"
#include "../render/Object3DRenderer.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Engine;
    class Camera;
    class Material;
    class Shader;
    class AttributeArrayBase;
    class Mesh;
    
    class MeshRenderer : public Object3DRenderer<Mesh> {
        friend class Engine;

    public:
        virtual ~MeshRenderer();
        virtual Bool forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        virtual Bool forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                         const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) override;
        Bool forwardRenderMesh(const ViewDescriptor& viewDescriptor, WeakPointer<Mesh> mesh, Bool isStatic,
                               const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        virtual Bool supportsRenderPath(RenderPath renderPath) override;
        virtual UInt32 getRenderQueueID() const override;
        void setMaterial(WeakPointer<Material> material);
        WeakPointer<Material> getMaterial();

    private:
        MeshRenderer(WeakPointer<Material> material, WeakPointer<Object3D> owner);
        void checkAndSetShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute checkAttribute,
                                        StandardAttribute setAttribute, WeakPointer<AttributeArrayBase> array, Bool force = false);
        void disableShaderAttribute(WeakPointer<Mesh> mesh, WeakPointer<Material> material, StandardAttribute attribute,
                                    WeakPointer<AttributeArrayBase> array);
        void setRenderStateForMaterial(WeakPointer<Material> material, Bool renderingDepthOutput);
        void setSkinningVars(WeakPointer<Mesh> mesh, WeakPointer<Material> material, WeakPointer<Shader> shader);
        void drawMesh(WeakPointer<Mesh> mesh);

        void testAndSetTexture2DWithInc(WeakPointer<Shader> shader, UInt32& textureSlot, Int32 shaderVarLoc, UInt32 textureID);
        void testAndSetTextureCubeWithInc(WeakPointer<Shader> shader, UInt32& textureSlot, Int32 shaderVarLoc, UInt32 textureID);

        PersistentWeakPointer<Material> material;
    };
}
