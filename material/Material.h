#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "../Graphics.h"
#include "Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

namespace Core {
  class Material {
  public:
    Material(WeakPointer<Graphics> graphics);
    Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader);
    WeakPointer<Shader> getShader();
    virtual Bool build() = 0;

    virtual Int32 getShaderLocation(StandardAttribute attribute) = 0;
    virtual Int32 getShaderLocation(StandardUniform uniform) = 0;
    virtual void sendCustomUniformsToShader() = 0;

  protected:
    PersistentWeakPointer<Graphics> graphics;
    PersistentWeakPointer<Shader> shader;
    Bool ready;
    Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void setShader(WeakPointer<Shader> shader);
  };
}
