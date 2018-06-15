#pragma once

#include "Material.h"
#include "../image/Texture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicTexturedMaterial : public Material {
    friend class Engine;

  public:
    
    virtual Bool build() override;
    virtual Int32 getShaderLocation(StandardAttributes attribute) override;
    virtual Int32 getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setTexture(std::shared_ptr<Texture> texture);

  private:
    BasicTexturedMaterial(std::shared_ptr<Graphics> graphics);
    
    std::shared_ptr<Texture> texture;
    Int32 positionLocation;
    Int32 colorLocation;
    Int32 textureLocation;
    Int32 uvLocation;
    Int32 projectionMatrixLocation;
    Int32 viewMatrixLocation;
  };
}
