#pragma once

#include "util/PersistentWeakPointer.h"
#include "Engine.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"
#include "material/BasicMaterial.h"

namespace Core {

  class Demo {

    std::vector<std::shared_ptr<StandardImage>> skyboxImages;
    std::shared_ptr<CubeTexture> skyboxTexture;
    PersistentWeakPointer<BasicMaterial> skyboxMaterial;

    std::shared_ptr<Core::ImageLoader> imageLoader;
    std::shared_ptr<Core::AssetLoader> assetLoader;

  public:

    Demo();
    void run();
  };

}
