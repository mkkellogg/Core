//
// Created by Mark Kellogg on 1/18/18.
//

#pragma once

#include "Engine.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"
#include "material/BasicCubeMaterial.h"

namespace Core {

  class Demo {
    Engine& engine;

    std::vector<std::shared_ptr<RawImage>> skyboxImages;
    std::shared_ptr<CubeTexture> skyboxTexture;
    std::shared_ptr<BasicCubeMaterial> skyboxMaterial;

    std::shared_ptr<Core::ImageLoader> imageLoader;
    std::shared_ptr<Core::AssetLoader> assetLoader;

  public:

    Demo(Engine& engine);
    void run();
  };

}
