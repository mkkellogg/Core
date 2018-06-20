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
    virtual Int32 getShaderLocation(StandardAttributes attribute) override;
    virtual Int32 getShaderLocation(StandardUniform uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setSkyboxTexture(std::shared_ptr<CubeTexture> texture);
    
  private:
    BasicCubeMaterial(WeakPointer<Graphics> graphics);
    GLint positionLocation;
    GLint colorLocation;
    GLint skyboxLocation;
    std::shared_ptr<CubeTexture> skyboxTexture;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
  };
}
