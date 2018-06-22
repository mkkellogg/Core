#pragma once

#include <memory>

#include "../Graphics.h"
#include "../util/PersistentWeakPointer.h"
#include "Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

namespace Core {

    // forward declarations
    class Engine;

    class Material {
    public:
        Material(Engine& engine, WeakPointer<Graphics> graphics);
        Material(Engine& engine, WeakPointer<Graphics> graphics, WeakPointer<Shader> shader);
        WeakPointer<Shader> getShader();
        virtual Bool build() = 0;

        virtual Int32 getShaderLocation(StandardAttribute attribute) = 0;
        virtual Int32 getShaderLocation(StandardUniform uniform) = 0;
        virtual void sendCustomUniformsToShader() = 0;
        virtual WeakPointer<Material> clone() = 0;

    protected:
        Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        void setShader(WeakPointer<Shader> shader);

        PersistentWeakPointer<Graphics> graphics;
        PersistentWeakPointer<Shader> shader;
        Bool ready;
        Engine& engine;
    };
}
