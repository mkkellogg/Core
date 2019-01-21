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
        
        Bool getDepthTestEnabled() const;
        void setDepthTestEnabled(Bool transparent);
        Bool hasCustomDepthTest();
        void setHasCustomDepthTest(Bool hasCustomDepth);
        
    protected:
        Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        Bool buildFromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource);
        void setShader(WeakPointer<Shader> shader);

        PersistentWeakPointer<Graphics> graphics;
        PersistentWeakPointer<Shader> shader;
        Bool ready;
        Bool transparent;
        Bool lit;
        RenderState::BlendingMode blendingMode;
        RenderState::BlendingMethod srcBlendingMethod;
        RenderState::BlendingMethod destBlendingMethod;
        RenderStyle renderStyle;
        Bool depthTestEnabled;

        Bool customDepthTestEnabled;
    };
}
