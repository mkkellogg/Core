#pragma once


#include "../util/WeakPointer.h"
#include "Material.h"
#include "../image/CubeTexture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicCubeMaterial : public Material {
    friend class Engine;

  public:
    
    virtual Bool build() override;
    virtual Int32 getShaderLocation(StandardAttribute attribute) override;
    virtual Int32 getShaderLocation(StandardUniform uniform) override;
    virtual void sendCustomUniformsToShader() override;
    virtual WeakPointer<Material> clone() override;
    void setSkyboxTexture(std::shared_ptr<CubeTexture> texture);
    
  private:
    BasicCubeMaterial(Engine& engine, WeakPointer<Graphics> graphics);
    Int32 positionLocation;
    Int32 colorLocation;
    Int32 skyboxLocation;
    Int32 projectionMatrixLocation;
    Int32 viewMatrixLocation;
    std::shared_ptr<CubeTexture> skyboxTexture;
  };
}
