#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"
#include "../image/Texture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicTexturedMaterial : public Material {
    friend class Engine;

  public:
    
    virtual Bool build() override;
    virtual Int32 getShaderLocation(StandardAttribute attribute) override;
    virtual Int32 getShaderLocation(StandardUniform uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setTexture(WeakPointer<Texture> texture);

  private:
    BasicTexturedMaterial(WeakPointer<Graphics> graphics);
    
    WeakPointer<Texture> texture;
    Int32 positionLocation;
    Int32 colorLocation;
    Int32 textureLocation;
    Int32 uvLocation;
    Int32 projectionMatrixLocation;
    Int32 viewMatrixLocation;
    GLint modelMatrixLocation;
  };
}
