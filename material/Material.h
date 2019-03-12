#pragma once

#include <memory>

#include "../Graphics.h"
#include "../util/PersistentWeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../render/RenderStyle.h"
#include "../render/RenderState.h"

namespace Core {

    // forward declarations
    class Shader;

    class Material {
    public:
        Material(WeakPointer<Graphics> graphics);
        Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader);
        WeakPointer<Shader> getShader();
        
        virtual Bool build() = 0;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) = 0;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) = 0;
        virtual void sendCustomUniformsToShader() = 0;
        virtual WeakPointer<Material> clone() = 0;
        virtual UInt32 textureCount();

        Bool getColorWriteEnabled() const;
        void setColorWriteEnabled(Bool enabled);
        RenderState::BlendingMethod getSourceBlendingMethod() const;
        void setSourceBlendingMethod(RenderState::BlendingMethod method);
        RenderState::BlendingMethod getDestBlendingMethod() const;
        void setDestBlendingMethod(RenderState::BlendingMethod method);
        RenderStyle getRenderStyle() const;
        void setRenderStyle(RenderStyle style);
        RenderState::BlendingMode getBlendingMode() const;
        void setBlendingMode(RenderState::BlendingMode mode);
        Bool isTransparent() const;
        void setTransparent(Bool transparent);
        Bool isLit() const;
        void setLit(Bool lit);
        Bool isPhysical() const;
        void setPhysical(Bool physical);
        
        Bool getDepthWriteEnabled() const;
        void setDepthWriteEnabled(Bool enabled);
        Bool getDepthTestEnabled() const;
        void setDepthTestEnabled(Bool enabled);
        RenderState::DepthFunction getDepthFunction();
        void setDepthFunction(RenderState::DepthFunction depthFunction);

        Bool getFaceCullingEnabled() const;
        void setFaceCullingEnabled(Bool enabled);
        RenderState::CullFace getCullFace();
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
        
    protected:
        virtual void copyTo(WeakPointer<Material> target);
        Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        Bool buildFromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource);
        void setShader(WeakPointer<Shader> shader);

        PersistentWeakPointer<Graphics> graphics;
        PersistentWeakPointer<Shader> shader;

    private:
        Bool colorWriteEnabled;
        RenderState::BlendingMode blendingMode;
        RenderState::BlendingMethod srcBlendingMethod;
        RenderState::BlendingMethod destBlendingMethod;
        RenderStyle renderStyle;
        Bool ready;
        Bool transparent;
        Bool lit;
        Bool physical;

        Bool stencilTestEnabled;
        Byte stencilRef;
        Byte stencilReadMask;
        Byte stencilWriteMask;
        RenderState::StencilFunction stencilComparisonFunction;
        RenderState::StencilAction stencilAllPassAction;
        RenderState::StencilAction stencilFailActionStencil;
        RenderState::StencilAction stencilFailActionDepth;

        Bool depthWriteEnabled;
        Bool depthTestEnabled;
        RenderState::DepthFunction depthFunction;

        Bool faceCullingEnabled;
        RenderState::CullFace cullFace;

    };
}
