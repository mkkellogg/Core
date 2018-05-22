#pragma once

#include <memory>
#include "../common/types.h"
#include "../scene/Object3D.h"

namespace Core {

  class AssetLoader {
  public:
    AssetLoader(){}
    virtual Byte * loadRawAsset(const std::string& path) = 0;
    virtual Byte * loadRawAsset(const std::string& path, UInt32& bytesRead) = 0;
    virtual Byte * loadRawFile(const std::string& path) = 0;
    virtual Byte * loadRawFile(const std::string& path, UInt32& bytesRead) = 0;
  };

}
