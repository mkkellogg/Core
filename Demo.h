//
// Created by Mark Kellogg on 1/18/18.
//

#ifndef MPM_DEMO_H
#define MPM_DEMO_H

#include "Engine.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"
#include "material/BasicCubeMaterial.h"

namespace MPM {

  class Demo {
    Engine& engine;

    std::vector<std::shared_ptr<RawImage>> skyboxImages;
    std::shared_ptr<CubeTexture> skyboxTexture;
    std::shared_ptr<BasicCubeMaterial> skyboxMaterial;

    std::shared_ptr<MPM::ImageLoader> imageLoader;
    std::shared_ptr<MPM::AssetLoader> assetLoader;

  public:

    Demo(Engine& engine);
    void run();
  };

}

#endif //MPM_DEMO_H
