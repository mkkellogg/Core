#pragma once

#include "Engine.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"
#include "material/BasicMaterial.h"

namespace Core {

  class Demo {
    Engine& engine;

    std::vector<std::shared_ptr<RawImage>> skyboxImages;
    std::shared_ptr<CubeTexture> skyboxTexture;
    WeakPointer<BasicMaterial> skyboxMaterial;

    std::shared_ptr<Core::ImageLoader> imageLoader;
    std::shared_ptr<Core::AssetLoader> assetLoader;

  public:

    Demo(Engine& engine);
    void run();
  };

}
