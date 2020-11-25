#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "../base/CoreObject.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "MaterialState.h"
#include "../render/EngineRenderQueue.h"

namespace Core {

    // forward declarations
    class Shader;
    class Graphics;
    class Texture;

    class Material : public CoreObject {
    public:
        Material();
        Material(WeakPointer<Shader> shader);
        virtual ~Material();
        WeakPointer<Shader> getShader();
        
        virtual Bool build() = 0;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) = 0;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) = 0;
        virtual void sendCustomUniformsToShader();
        virtual WeakPointer<Material> clone() = 0;
        virtual UInt32 textureCount();

        Bool getColorWriteEnabled() const;
        void setColorWriteEnabled(Bool enabled);
        RenderState::BlendingEquation getBlendingEquation() const;
        void setBlendingEquation(RenderState::BlendingEquation equation);
        RenderState::BlendingFactor getSourceBlendingFactor() const;
        void setSourceBlendingFactor(RenderState::BlendingFactor factor);
        RenderState::BlendingFactor getDestBlendingFactor() const;
        void setDestBlendingFactor(RenderState::BlendingFactor factor);
        RenderStyle getRenderStyle() const;
        void setRenderStyle(RenderStyle style);
        RenderPath getRenderPath() const;
        void setRenderPath(RenderPath path);
        RenderState::BlendingMode getBlendingMode() const;
        void setBlendingMode(RenderState::BlendingMode mode);
        UInt16 getRenderQueueID() const;
        void setRenderQueue(EngineRenderQueue queue);
        void setRenderQueueID(UInt16 renderQueueID);
        Bool isLit() const;
        void setLit(Bool lit);
        virtual UInt32 maxLightCount() const;
        Bool isPhysical() const;
        void setPhysical(Bool physical);
        Bool isSkinningEnabled() const;
        void setSkinningEnabled(Bool enabled);
        
        Bool getDepthWriteEnabled() const;
        void setDepthWriteEnabled(Bool enabled);
        Bool getDepthTestEnabled() const;
        void setDepthTestEnabled(Bool enabled);
        RenderState::DepthFunction getDepthFunction();
        void setDepthFunction(RenderState::DepthFunction depthFunction);

        Bool getFaceCullingEnabled() const;
        void setFaceCullingEnabled(Bool enabled);
        RenderState::CullFace getCullFace() const;
        void setCullFace(RenderState::CullFace cullFace);

        Bool getStencilTestEnabled() const;
        void setStencilTestEnabled(Bool enabled);
        Byte getStencilRef() const;
        void setStencilRef(Byte ref);
        Byte getStencilReadMask() const;
        void setStencilReadMask(Byte mask);
        Byte getStencilWriteMask() const;
        void setStencilWriteMask(Byte mask);
        RenderState::StencilFunction getStencilComparisonFunction() const;
        void setStencilComparisonFunction(RenderState::StencilFunction function);
        RenderState::StencilAction getStencilAllPassAction() const;
        void setStencilAllPassAction(RenderState::StencilAction action);
        RenderState::StencilAction getStencilFailActionStencil() const;
        void setStencilFailActionStencil(RenderState::StencilAction action);
        RenderState::StencilAction getStencilFailActionDepth() const;
        void setStencilFailActionDepth(RenderState::StencilAction action);

        void setCustomDepthOutput(Bool hasCustomDepthOutput);
        Bool hasCustomDepthOutput() const;
        Bool getCustomDepthOutputCopyOverrideMatrialState() const;
        void setCustomDepthOutputCopyOverrideMatrialState(Bool state);
        Bool getCustomDepthOutputStateCopyExcludeFaceCulling() const;
        void setCustomDepthOutputStateCopyExcludeFaceCulling(Bool exclude);

        virtual Bool hasOpacityMap() const;
        virtual WeakPointer<Texture> getOpacityMap();

        MaterialState getState() const;
        void setState(MaterialState state);
        
    protected:
        virtual void copyTo(WeakPointer<Material> target);
        Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        Bool buildFromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource);
        void setShader(WeakPointer<Shader> shader);

        PersistentWeakPointer<Shader> shader;

    private:
        Bool ready;
        Bool lit;
        Bool physical;
        Bool skinningEnabled;
        RenderPath renderPath;
        UInt16 renderQueueID;
        MaterialState materialState;
        Bool customDepthOutput;
        Bool customDepthOutputCopyOverrideMaterialState;
        Bool customDepthOutputStateCopyExcludeFaceCulling;
    };
}
