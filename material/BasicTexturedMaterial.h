#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

  // forward declarations
  class Engine;
  class Texture;

  class BasicTexturedMaterial : public Material {
    friend class Engine;

  public:
    
    virtual Bool build() override;
    virtual Int32 getShaderLocation(StandardAttribute attribute) override;
    virtual Int32 getShaderLocation(StandardUniform uniform) override;
    virtual void sendCustomUniformsToShader() override;
    virtual WeakPointer<Material> clone() override;
    void setTexture(WeakPointer<Texture> texture);

  private:
    BasicTexturedMaterial(Engine& engine, WeakPointer<Graphics> graphics);
    
    WeakPointer<Texture> texture;
    Int32 positionLocation;
    Int32 colorLocation;
    Int32 textureLocation;
    Int32 uvLocation;
    Int32 projectionMatrixLocation;
    Int32 viewMatrixLocation;
    Int32 modelMatrixLocation;
  };
}
